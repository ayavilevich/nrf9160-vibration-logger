#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <stdlib.h>
#include <math.h>

#include "vibration.h"

#define ACCELEROMETER_CHANNELS 3
#define SAMPLES_IN_MEASUREMENT 100

LOG_MODULE_REGISTER(vibration, CONFIG_MQTT_MULTI_SERVICE_LOG_LEVEL);

static const struct device *accel_sensor = DEVICE_DT_GET(DT_ALIAS(accelerometer));

static struct sensor_trigger accel_sensor_trigger = {
		.chan = SENSOR_CHAN_ACCEL_XYZ,
		.type = SENSOR_TRIG_DATA_READY
};

double previous_sample[ACCELEROMETER_CHANNELS] = {0, 0, 0};
int samples_to_make = 0;
double measurement = 0;
double measurement_in_progress = 0;

static void accel_trigger_handler(const struct device *dev,
				   const struct sensor_trigger *trig)
{
	struct sensor_value data[ACCELEROMETER_CHANNELS];
	// struct ext_sensor_evt evt = {0};
	int err;

	switch (trig->type) {
	case SENSOR_TRIG_DATA_READY:
		if (sensor_sample_fetch(dev) < 0) {
			LOG_ERR("Sample fetch error");
			return;
		}

		err = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, data);
		if (err) {
			LOG_ERR("sensor_channel_get, error: %d", err);
			return;
		}
		// convert from sensor_value to regular numbers. TBD if this is sub optimal.
		// LOG_DBG("%d %d %d, %d %d %d", data[0].val1, data[1].val1, data[2].val1, data[0].val2, data[1].val2, data[2].val2);
		double sample[ACCELEROMETER_CHANNELS];
		sample[0] = sensor_value_to_double(&data[0]);
		sample[1] = sensor_value_to_double(&data[1]);
		sample[2] = sensor_value_to_double(&data[2]);
		// LOG_DBG("%f %f %f", sample[0], sample[1], sample[2]);

		if (samples_to_make <= 0) // start a new measurement
		{
			measurement = measurement_in_progress; // finalize in-progress measurement
			LOG_DBG("%6.2f", measurement);
			samples_to_make = SAMPLES_IN_MEASUREMENT;
			measurement_in_progress = 0;
		}
		measurement_in_progress += fabs(sample[0] - previous_sample[0]) + fabs(sample[1] - previous_sample[1])+ fabs(sample[2] - previous_sample[2]); // measure diff from prev sample
		samples_to_make--;

		// store current as previous
		memcpy(previous_sample, sample, sizeof(previous_sample));

		// evt.value = sqrt(pow(sensor_ms2_to_g(&data[0]), 2.0) +
		// 		 pow(sensor_ms2_to_g(&data[1]), 2.0) +
		// 		 pow(sensor_ms2_to_g(&data[2]), 2.0));

		// LOG_DBG("%f %f %f", sensor_value_to_double(&data[0]), sensor_value_to_double(&data[1]), sensor_value_to_double(&data[2]));
		// LOG_DBG("Detected impact of %6.2f g\n", evt.value);

		// if (evt.value > 0.0) {
		// 	evt.type = EXT_SENSOR_EVT_ACCELEROMETER_IMPACT_TRIGGER;
		// 	evt_handler(&evt);
		// }
		break;
	default:
		LOG_ERR("Unknown trigger");
	}
}

void start_vibration_tracking()
{
	if (!device_is_ready(accel_sensor)) {
		LOG_ERR("Accelerometer device is not ready");
	} else {
		int err = sensor_trigger_set(accel_sensor, &accel_sensor_trigger, accel_trigger_handler);
		if (err) {
			LOG_ERR("Could not set trigger for device %s, error: %d",
				accel_sensor->name, err);
			// return err;
		}
	}
}

double get_vibration_measurement()
{
	return measurement;
}