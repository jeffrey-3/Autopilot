#ifndef TELEM_H_
#define TELEM_H_

#include "lib/utils/utils.h"
#include "hal.h"
#include "modes.h"
#include "params.h"
#include "constants.h"
#include "module.h"
#include <cstdio>
#include <cstring>

extern "C"
{
#include "lib/aplink_c/aplink.h"
#include "lib/aplink_c/aplink_messages.h"
}

// Rate to transmit messages
static constexpr float VEHICLE_STATUS_FULL_DT = 0.03;
static constexpr float GPS_RAW_DT = 0.2;
static constexpr float POWER_DT = 1;
static constexpr float CAL_SENSORS_DT = 0.1;

enum class TelemState
{
	LOAD_PARAMS,
	LOAD_WAYPOINTS,
	SEND_TELEMETRY
};

class Telem : public Module
{
public:
	Telem(HAL* hal, Data_bus* data_bus);

	void update();

private:
	Subscriber<GNSS_data> _gnss_sub;
	Subscriber<AHRS_data> _ahrs_sub;
	Subscriber<local_position_s> _local_pos_sub;
	Subscriber<Modes_data> _modes_sub;
	Subscriber<position_control_s> _position_control_sub;
	Subscriber<waypoint_s> _waypoint_sub;
	Subscriber<Power_data> _power_sub;
	Subscriber<Ctrl_cmd_data> _ctrl_cmd_sub;
	Subscriber<Baro_data> _baro_sub;
	Subscriber<IMU_data> _imu_sub;

	Publisher<Telem_data> _telem_pub;
	Publisher<telem_new_waypoint_s> _telem_new_waypoint_pub;

	Ctrl_cmd_data _ctrl_cmd_data;
	AHRS_data _ahrs_data;
	GNSS_data _gnss_data;
	Telem_data _telem_data;
	local_position_s _local_pos;
	Modes_data _modes_data;
	position_control_s  _position_control;
	Power_data _power_data;
	Baro_data _baro_data;
	IMU_data _imu_data;

	aplink_msg telem_msg;

	TelemState _telem_state;
	float last_vehicle_status_full_transmit_s = 0;
	float last_gps_raw_transmit_s = 0;
	float last_cal_sensors_transmit_s = 0;
	float last_power_transmit_s = 0;

	uint8_t _num_waypoints = 0; // Number of waypoints to load
	uint8_t _last_waypoint_loaded = 0;

	void update_load_params();
	void update_load_waypoints();
	void update_send_telemetry();
	void update_calibration();

	bool read_telem(aplink_msg* msg);
	void read_usb();
	void transmit_usb();
	void transmit_packet(uint8_t packet[], uint16_t size);
	bool parse_packet();
};

#endif /* TELEM_H_ */
