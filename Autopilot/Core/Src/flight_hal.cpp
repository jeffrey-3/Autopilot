#include <flight_hal.h>

Flight_hal::Flight_hal(Plane * plane) : HAL(plane),
										  _imu(&hspi1, GPIOC, GPIO_PIN_15, SPI_BAUDRATEPRESCALER_128, SPI_BAUDRATEPRESCALER_4),
										  _ina219(&hi2c1, 0.01),
										  _gnss(&huart3)
{
	_plane = plane;
}

void Flight_hal::init()
{
	init_imu();
	init_baro();
	init_compass();
	init_gnss();
	init_logger();
}

void Flight_hal::read_sensors()
{
	read_imu();
	read_baro();
	read_compass();
	read_gnss();
	read_power_monitor();
}
