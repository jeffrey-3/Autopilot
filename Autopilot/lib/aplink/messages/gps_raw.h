#ifndef LIB_APLINK_MESSAGES_GPS_RAW_H_
#define LIB_APLINK_MESSAGES_GPS_RAW_H_

#include "aplink.h"

struct aplink_gps_raw
{
	int32_t lat;
	int32_t lon;
	uint8_t sats;
	bool fix;
};

#endif /* LIB_APLINK_MESSAGES_GPS_RAW_H_ */
