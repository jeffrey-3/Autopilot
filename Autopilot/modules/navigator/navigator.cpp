#include "modules/navigator/navigator.h"

Navigator::Navigator(HAL* hal)
	: Module(hal),
	  _pos_est_sub(Data_bus::get_instance().pos_est_node),
	  _telem_sub(Data_bus::get_instance().telem_node),
	  _navigator_pub(Data_bus::get_instance().navigator_node)
{
}

void Navigator::update()
{
	if (_pos_est_sub.check_new())
	{
		Pos_est_data pos_est_data = _pos_est_sub.get();
		Telem_data telem_data = _telem_sub.get();

		// Get target waypoint
		const Waypoint& target_wp = telem_data.waypoints[_curr_wp_idx];

		// Convert waypoint to north east coordinates
		double tgt_north, tgt_east;
		lat_lon_to_meters(Data_bus::get_home(telem_data).lat, Data_bus::get_home(telem_data).lon,
						  target_wp.lat, target_wp.lon,
						  &tgt_north, &tgt_east);

		// Check distance to waypoint to determine if waypoint reached
		float rel_east = pos_est_data.pos_e - tgt_east;
		float rel_north = pos_est_data.pos_n - tgt_north;
		float dist_to_wp = sqrtf(rel_north*rel_north + rel_east*rel_east);

		if ((dist_to_wp < get_params()->navigator.min_dist_wp) &&
			(_curr_wp_idx < telem_data.num_waypoints - 1))
		{
			_curr_wp_idx++; // Move to next waypoint

			Navigator_data navigator_data;
			navigator_data.waypoint_index = _curr_wp_idx;
			navigator_data.timestamp = _hal->get_time_us();

			_navigator_pub.publish(navigator_data);
		}
	}
}
