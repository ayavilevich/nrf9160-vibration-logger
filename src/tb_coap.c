#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/socket.h>
#include <net/coap_utils.h>

#include "tb_coap.h"

LOG_MODULE_REGISTER(tb_coap, CONFIG_MQTT_MULTI_SERVICE_LOG_LEVEL);

static struct sockaddr server;

static int server_resolve(void)
{
	int err;
	struct addrinfo *result;
	struct addrinfo hints = {
		.ai_family = AF_INET,
		.ai_socktype = SOCK_DGRAM
	};

	err = getaddrinfo(CONFIG_CLOUD_THINGSBOARD_COAP_SERVER_HOSTNAME, NULL, &hints, &result);
	if (err != 0) {
		LOG_ERR("getaddrinfo failed %d\n", err);
		return -EIO;
	}

	if (result == NULL) {
		LOG_ERR("Address not found\n");
		return -ENOENT;
	}

	/* IPv4 Address. */
	struct sockaddr_in *server4 = ((struct sockaddr_in *)&server);

	server4->sin_addr.s_addr = ((struct sockaddr_in *)result->ai_addr)->sin_addr.s_addr;
	server4->sin_family = AF_INET;
	server4->sin_port = htons(CONFIG_CLOUD_THINGSBOARD_COAP_SERVER_PORT);

	// print
	char ipv4_addr[NET_IPV4_ADDR_LEN];
	inet_ntop(AF_INET, &server4->sin_addr.s_addr, ipv4_addr, sizeof(ipv4_addr));
	LOG_DBG("IPv4 Address found %s", ipv4_addr);

	/* Free the address. */
	freeaddrinfo(result);

	return 0;
}

int tb_coap_reply(const struct coap_packet *response, struct coap_reply *reply, const struct sockaddr *from)
{
	LOG_INF("tb_coap_reply");
	return 0;
}

void start_tb_coap()
{
	if (server_resolve() != 0) {
		LOG_ERR("Failed to resolve server name");
	}

	coap_init(server.sa_family, NULL);
}

int tb_coap_send_telemetry(const char * name, int value)
{
	LOG_DBG("coap name: %s", name);
	// format as JSON. this is a simple case, so don't use a serialization library
	char payload[10 + strlen(name) + 10]; // {"name": value}
	snprintf(payload, sizeof(payload), "{\"%s\":%d}", name, value);
	LOG_DBG("coap payload: %s", payload);
	// make path
	char path[20 + strlen(CONFIG_CLOUD_THINGSBOARD_COAP_ACCESS_TOKEN)];
	snprintf(path, sizeof(path), "api/v1/%s/telemetry", CONFIG_CLOUD_THINGSBOARD_COAP_ACCESS_TOKEN);
	LOG_DBG("coap path: %s", path);
	// send
	const char * uri_path_options[2] = {path, NULL};
	int ret = coap_send_request(COAP_METHOD_POST, &server, uri_path_options, (uint8_t *)payload, strlen(payload), /*tb_coap_reply*/NULL);
	LOG_DBG("coap_send_request: %d", ret);
	return ret;
}
