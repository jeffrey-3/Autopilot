#ifndef MODULES_STORAGE_STORAGE_H_
#define MODULES_STORAGE_STORAGE_H_

// TODO: Add COBS and start byte

#include "plane.h"
#include "hal.h"
#include <stdint.h>
#include <cstring>

struct __attribute__((packed))Storage_packet
{
	uint64_t time;
	float acc_z;
	float alt;
};

class Storage
{
public:
	Storage(Plane* plane, HAL* hal);

	void write();
	void flush();
	void read();

private:
	Plane* _plane;
	HAL* _hal;
};

#endif /* MODULES_STORAGE_STORAGE_H_ */
