/*
 * parameters.h
 *
 *  Created on: Jan. 29, 2025
 *      Author: jeffr
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

// Airspeed
constexpr float AIRSPEED_CRUISE = 18; // Meters per second

// Throttle
constexpr float TRIM_THROTTLE = 0.3; // Steady-state cruise throttle
constexpr float THR_MIN = 0; // Maximum throttle between 0 and 1
constexpr float THR_MAX = 1; // Minimum throttle between 0 and 1

// Attitude
constexpr float PTCH_LIM_MAX_DEG = 10;
constexpr float PTCH_LIM_MIN_DEG = -10;
constexpr float ROLL_LIM_DEG = 15; // Maximum roll angle in either direction
constexpr bool RUDDER_ONLY = true;

// Takeoff
constexpr float TAKEOFF_ALT = 10; // Altitude that the plane will climb to during takeoff meters per second
constexpr float TAKEOFF_THR = 1; // Throttle set during takeoff between 0 and 1
constexpr float TAKEOFF_AIRSPD = 10; // Rotation speed meters per second during takeoff, ignored during hand launch
constexpr float TAKEOFF_PTCH = 10; // Pitch during takeoff

// Launch detection
constexpr float LAUN_ACC_THLD = 10; // Acceleration in body-forward direction to detect takeoff
constexpr float LAUN_ACC_TIME = 0.1; // Trigger time (acceleration must be above threshold for this amount of seconds to detect takeoff)
constexpr float LAUN_MOT_DEL = 2; // Delay from launch detection to motor spin up

// Guidance
constexpr float MIN_DIST_WP = 100; // Distance in meters from waypoint until switching to next, "radius of acceptance"
constexpr uint8_t MAX_NUM_WPTS = 100; // Maximum number of waypoints

// Radio
constexpr uint8_t TELEM_PKT_LEN = 40;

#endif /* PARAMETERS_H_ */
