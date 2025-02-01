#include "telem.h"

#include <cstdio> // Temporary for printf debug

Telem::Telem(HAL* hal, Plane* plane)
{
	_plane = plane;
	_hal = hal;
}

void Telem::transmit()
{
	Telem_payload payload;
	payload.roll = _plane->ahrs_roll;
	payload.pitch = _plane->ahrs_pitch;
	payload.yaw = _plane->ahrs_yaw;
	payload.alt = -_plane->nav_pos_down;
	payload.spd = _plane->nav_airspeed;
	payload.lat = _plane->gnss_lat;
	payload.lon = _plane->gnss_lon;

	uint8_t payload_arr[sizeof(Telem_payload)];
	memcpy(payload_arr, &payload, sizeof(Telem_payload));

	uint8_t packet_no_start_byte[TELEM_PKT_LEN - 1]; // Packet without start byte, therefore subtract 1
	cobs_encode(packet_no_start_byte, sizeof(packet_no_start_byte), payload_arr, sizeof(Telem_payload));

	// Add start byte to packet
	uint8_t packet[TELEM_PKT_LEN];
	packet[0] = 0;
	for (int i = 1; i < TELEM_PKT_LEN; i++)
	{
		packet[i] = packet_no_start_byte[i - 1];
	}

	_hal->transmit_telem(packet, TELEM_PKT_LEN);
}

// Send back same message
void Telem::acknowledgement()
{
	_hal->transmit_telem(_plane->latest_packet, TELEM_PKT_LEN);
}

void Telem::parse_telemetry()
{
	// Remove start byte
	uint8_t packet_no_start_byte[TELEM_PKT_LEN - 1];
	for (int i = 0; i < TELEM_PKT_LEN - 1; i++)
	{
		packet_no_start_byte[i] = _plane->latest_packet[i + 1];
	}

	uint8_t payload[TELEM_PKT_LEN - 2]; // Subtract two since removed start byte and COBS byte
	cobs_decode(payload, sizeof(payload), packet_no_start_byte, sizeof(packet_no_start_byte));

	if (payload[0] == 1) // Command payload
	{
		Command_payload command_payload;
		memcpy(&command_payload, payload, sizeof(Command_payload));
		printf("%d\n", command_payload.command);
	}
	else if (payload[0] == 2) // Waypoint payload
	{
		Waypoint_payload waypoint_payload;
		memcpy(&waypoint_payload, payload, sizeof(Waypoint_payload));
		printf("%f %f %f\n", waypoint_payload.waypoint[0], waypoint_payload.waypoint[1], waypoint_payload.waypoint[2]);
	}
}

void Telem::update()
{
	// If new recieved command, send acknowledgement
	// Otherwise send telemetry packet
	if (_hal->read_telem())
	{
		parse_telemetry();
		acknowledgement();
	}
	else
	{
		transmit();
	}
}
