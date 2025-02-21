/**
 * A simple library to control ZLAC8015D Motor Driver
 * This libarary is using on ModbusMaster libarary to
 * help the communication of RS485 Modbus RTU.
 * 
 * This libaray is to make the set/get of command to
 * ZLAC8015D motor driver more convenient.
 * 
 * by Rasheed Kittinanthapanya
 * */

#include "ZLAC8015D.h"


// ZLAC8015D::ZLAC8015D(ModbusMaster *node)
ZLAC8015D::ZLAC8015D()
{
	// _node = node;
	// _ser = ser;

}


void ZLAC8015D::set_modbus(ModbusMaster *node){
	/**
	 * Copy ModbusMaster object (node) into class
	 *
	 * */
	_node = node;
}

uint8_t ZLAC8015D::disable_motor(){
	/**
	 * Disable motor
	 * This should run every time before start everything
	 * to make sure the motor starts from stop.
	 * Torque will be release from wheels
	 * */
	result = _node->writeSingleRegister(CONTROL_REG, DOWN_TIME);
	return result;
}

uint8_t ZLAC8015D::set_mode(uint8_t mode){
	/**
	 * Set motor driver mode
	 * mode: 1 is position control, 3 is velocity control
	 * */
	result = _node->writeSingleRegister(OPR_MODE, mode);
	return result;
}

uint8_t ZLAC8015D::enable_motor(){
	/**
	 * Enable motor
	 * After disable motor, then this function is called to start motor.
	 * You can feel the torque is applied on the wheels
	 * */
	result = _node->writeSingleRegister(CONTROL_REG, ENABLE);
	return result;
}

uint8_t ZLAC8015D::set_accel_time(uint16_t L_ms, uint16_t R_ms){
	/**
	 * Set acceleration time of each wheel
	 * unit is in milliseconds
	 * */
	L_ms = constrain(L_ms, 0, 32767);
	R_ms = constrain(R_ms, 0, 32767);

	_node->setTransmitBuffer(0, L_ms);
	_node->setTransmitBuffer(1, R_ms);

	result = _node->writeMultipleRegisters(L_ACL_TIME, 2);

	return result;

}

uint8_t ZLAC8015D::set_decel_time(uint16_t L_ms, uint16_t R_ms){
	/**
	 * Set deceleration time of each wheel
	 * unit is in milliseconds
	 * */
	L_ms = constrain(L_ms, 0, 32767);
	R_ms = constrain(R_ms, 0, 32767);

	_node->setTransmitBuffer(0, L_ms);
	_node->setTransmitBuffer(1, R_ms);

	result = _node->writeMultipleRegisters(L_DCL_TIME, 2);

	return result;
}

uint8_t ZLAC8015D::set_maxRPM_pos(uint16_t max_L_rpm, uint16_t max_R_rpm){
	/**
	 * Set maximum speed in position control mode
	 * */
	max_L_rpm = constrain(max_L_rpm, 1, 1000);
	max_R_rpm = constrain(max_R_rpm, 1, 1000);

	_node->setTransmitBuffer(0, max_L_rpm);
	_node->setTransmitBuffer(1, max_R_rpm);

	result = _node->writeMultipleRegisters(L_MAX_RPM_POS, 2);

	return result;

}

uint8_t ZLAC8015D::set_rpm(int16_t L_rpm, int16_t R_rpm){
	/**
	 * Set RPM value of each wheel
	 * Note: R_rpm will be reverse sign,
	 * in order to make it spin correctly on cart.
	 * */

	L_rpm = constrain(L_rpm, -3000, 3000);
	R_rpm = constrain(-R_rpm, -3000, 3000);

	_node->setTransmitBuffer(0, L_rpm);
	_node->setTransmitBuffer(1, R_rpm);

	result = _node->writeMultipleRegisters(L_CMD_RPM, 2);

	return result;
}

uint8_t ZLAC8015D::get_rpm(int16_t res[2]){
	/**
	 * Get RPM of wheel wheel
	 * res[2] is passed by reference from user, defined on sketch
	 * the right feedback RPM res[1] is reverse sign in order to
	 * get correct rotation for cart.
	 * */
	result = _node->readHoldingRegisters(L_FB_RPM, 2);

	if (result == 0){
	    for (int j = 0; j < 2; j++)
	    {
	      res[j] = ( (int16_t)(_node->getResponseBuffer(j)) )/10;
	    }


	    // reverse sign of right wheel
	    res[1] = -res[1];
	}

	return result;
}

uint8_t ZLAC8015D::get_fault_code(int16_t res[2]){
	/*
		Get fault code

		NO_FAULT = 0x0000       0
		OVER_VOLT = 0x0001      1
		UNDER_VOLT = 0x0002     2
		OVER_CURR = 0x0004      4
		OVER_LOAD = 0x0008      8
		CURR_OUT_TOL = 0x0010   16
		ENCOD_OUT_TOL = 0x0020  32
		MOTOR_BAD = 0x0040      64
		REF_VOLT_ERROR = 0x0080 128
		EEPROM_ERROR = 0x0100   256
		WALL_ERROR = 0x0200     512
		HIGH_TEMP = 0x0400      1024
	
	*/

	result = _node->readHoldingRegisters(L_FAULT, 2);

	if (result == 0){
	    for (int j = 0; j < 2; j++)
	    {
	      res[j] = (int16_t)(_node->getResponseBuffer(j));
	    }
	}
	return result;

}

uint8_t ZLAC8015D::get_encoder_count(int32_t res[2]){
	/*
		Get wheel encoder raw count
	*/
	int16_t left_hi;
	int16_t left_lo;
	int16_t right_hi;
	int16_t right_lo;

	result = _node->readHoldingRegisters(L_FB_POS_HI, 4);

	if (result == 0){

	    left_hi = (int16_t)(_node->getResponseBuffer(0));
	    left_lo = (int16_t)(_node->getResponseBuffer(1));
	    right_hi = (int16_t)(_node->getResponseBuffer(2));
	    right_lo = (int16_t)(_node->getResponseBuffer(3));

	    res[0] = (((left_hi & 0xFFFF) << 16) | (left_lo & 0xFFFF));
	    res[1] = (((right_hi & 0xFFFF) << 16) | (right_lo & 0xFFFF));

	}

	return result;

}