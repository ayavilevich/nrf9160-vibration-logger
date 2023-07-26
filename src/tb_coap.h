void start_tb_coap();
int tb_coap_send_telemetry_int(const char * name, int value);
int tb_coap_send_telemetry_double(const char * name, double value);
int tb_coap_send_telemetry_payload_string(const char * name, const char * payload);
