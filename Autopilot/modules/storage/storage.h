#ifndef MODULES_STORAGE_STORAGE_H_
#define MODULES_STORAGE_STORAGE_H_

#include <data_bus.h>
#include "lib/cobs/cobs.h"
#include "hal.h"
#include "module.h"
#include <stdint.h>
#include <cstring>
#include <stdio.h>

struct __attribute__((packed))Storage_payload
{
	uint32_t loop_iteration;
	uint64_t time;
	float gyro[3];
	float accel[3];
	float mag_uncalib[3];
	float nav_pos[3];
	float nav_vel[3];
	float baro_alt;
	float rc_channels[4];
	double gnss_lat;
	double gnss_lon;
	bool gps_fix;
	uint8_t mode_id;
};

class Storage : public Module
{
public:
	Storage(HAL* hal, Plane* plane);

	void update();
	void update_background();

private:
	static constexpr int payload_size = sizeof(Storage_payload);
	static constexpr int packet_size = payload_size + 2; // Add start byte and COBS
	static constexpr int buffer_size = 200 * packet_size;
	uint8_t front_buffer[buffer_size];
	uint8_t back_buffer[buffer_size];
	bool front_buff_full = false;
	uint32_t back_buff_last_idx = 0;

	Plane::Subscription_handle imu_handle;
	Plane::Subscription_handle mag_handle;
	Plane::Subscription_handle gnss_handle;
	Plane::Subscription_handle baro_handle;
	Plane::Subscription_handle pos_est_handle;

	Plane::IMU_data imu_data;
	Plane::Mag_data mag_data;
	Plane::GNSS_data gnss_data;
	Plane::Pos_est_data pos_est_data;
	Plane::Baro_data baro_data;

	void write();
	void flush();
	void read();
	Storage_payload create_payload();
};

#endif /* MODULES_STORAGE_STORAGE_H_ */
