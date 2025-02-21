

#ifndef ZLAC8015D_H
#define ZLAC8015D_H

#include "Arduino.h"
#include <ModbusMaster.h>

// Common
#define CONTROL_REG 0x200E
#define OPR_MODE  0x200D
#define L_ACL_TIME  0x2080
#define R_ACL_TIME  0x2081
#define L_DCL_TIME  0x2082
#define R_DCL_TIME  0x2083

// Velocity control
#define L_CMD_RPM  0x2088
#define R_CMD_RPM  0x2089
#define L_FB_RPM  0x20AB
#define R_FB_RPM  0x20AC

// Position control
#define POS_CONTROL_TYPE  0x200F

#define L_MAX_RPM_POS  0x208E
#define R_MAX_RPM_POS  0x208F

#define L_CMD_REL_POS_HI  0x208A
#define L_CMD_REL_POS_LO  0x208B
#define R_CMD_REL_POS_HI  0x208C
#define R_CMD_REL_POS_LO  0x208D

#define L_FB_POS_HI  0x20A7
#define L_FB_POS_LO  0x20A8
#define R_FB_POS_HI  0x20A9
#define R_FB_POS_LO  0x20AA

#define L_FAULT 0x20A5
#define R_FAULT 0x20A6

// Control command
#define EMER_STOP  0x05
#define ALRM_CLR 0x06
#define DOWN_TIME 0x07
#define ENABLE 0x08
#define POS_SYNC 0x10
#define POS_L_START 0x11
#define POS_R_START 0x12


class ZLAC8015D
{
public:
	// ZLAC8015D(ModbusMaster *node, Stream *ser);
	// ZLAC8015D(ModbusMaster *node);
	ZLAC8015D();

	void set_modbus(ModbusMaster *node);
	uint8_t disable_motor();
	uint8_t set_mode(uint8_t mode);
	uint8_t enable_motor();
	uint8_t set_accel_time(uint16_t L_ms, uint16_t R_ms);
	uint8_t set_decel_time(uint16_t L_ms, uint16_t R_ms);
	uint8_t set_maxRPM_pos(uint16_t max_L_rpm, uint16_t max_R_rpm);
	uint8_t set_rpm(int16_t L_rpm, int16_t R_rpm);
	uint8_t get_rpm(int16_t res[2]);
	uint8_t get_fault_code(int16_t res[2]);
	uint8_t get_encoder_count(int32_t res[2]);


private:
	ModbusMaster* _node;

	uint8_t result;




};

#endif