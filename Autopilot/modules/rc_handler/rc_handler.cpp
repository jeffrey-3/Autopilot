#include "modules/rc_handler/rc_handler.h"

RCHandler::RCHandler(HAL* hal, DataBus* data_bus)
	: Module(hal, data_bus),
	  _modes_sub(data_bus->modes_node),
	  _rc_pub(data_bus->rc_node)
{
}

void RCHandler::update()
{
	_modes_data = _modes_sub.get();

	if (_modes_data.system_mode != System_mode::LOAD_PARAMS)
	{
		int32_t min_duty, max_duty;

		param_get(RC_MIN_DUTY, &min_duty);
		param_get(RC_MAX_DUTY, &max_duty);

		// Get RC input duty cycle
		uint16_t rc_input[NUM_CH];
		_hal->get_rc_input(rc_input, NUM_CH);

		// Convert to values from -1 to 1
		_rc_data.ail_norm = map(rc_input[AIL_CH], min_duty, max_duty, -1, 1);
		_rc_data.ele_norm = map(rc_input[ELE_CH], min_duty, max_duty, -1, 1);
		_rc_data.rud_norm = map(rc_input[RUD_CH], min_duty, max_duty, -1, 1);
		_rc_data.thr_norm = map(rc_input[THR_CH], min_duty, max_duty, 0, 1);

		uint16_t midpoint = min_duty + (max_duty - min_duty) / 2;
		_rc_data.man_sw = rc_input[MAN_CH] > midpoint;
		_rc_data.mod_sw = rc_input[MOD_CH] > midpoint;

		// TODO: Need more proper way to detect tx connected
		_rc_data.tx_conn = rc_input[THR_CH] > 500;

		_rc_pub.publish(_rc_data);
	}
}
