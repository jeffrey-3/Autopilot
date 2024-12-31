#include <flight_hal.h>
#include "plane.h"
#include "pitl_hal.h"
#include <scheduler.h>
#include "autopilot.h"

#define PITL_ENABLE false

Plane plane;

#if PITL_ENABLE
Pitl_hal hal;
#else
Flight_hal hal(&plane);
#endif

Autopilot autopilot(&hal, &plane);

void main_cpp()
{
	autopilot.init();

	if (HAL_TIM_Base_Start_IT(&htim7) != HAL_OK)
	{
		Error_Handler();
	}

	while (1)
	{
		autopilot.logger_task();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim7)
	{
		autopilot.main_task();
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	if (huart == &huart3)
	{
		hal.gnss_dma_complete();
	}
}

extern "C"
{
void main_c()
{
	main_cpp();
}
}
