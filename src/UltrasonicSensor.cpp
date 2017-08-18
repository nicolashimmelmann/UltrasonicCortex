/*
 * UltrasonicSensor.cpp
 *
 *  Created on: Aug 8, 2017
 *      Author: felix
 */

#include "include/UltrasonicSensor.h"


UltrasonicSensor::UltrasonicSensor(EXT * ext, short tP, short eP, bool needsPWM) {
	this->ext = ext;
	this->triggerPin = tP;
	this->echoPin = eP;
	this->needsPWM = needsPWM;
}

void UltrasonicSensor::init() {
	if(needsPWM)
	{
		//If PWM is needed, the pin is fixed.
		//Then set the echo pin on GPIO0 (PB15) to input
		//Then it is an ARX-ULT10 sensor
		palSetPadMode(GPIOB, echoPin, PAL_MODE_INPUT);
		pwm.init(40000);
	}
	else
	{
		//Set pin modes for HC-SR04
		palSetPadMode(GPIOB, triggerPin, PAL_MODE_OUTPUT_PUSHPULL);
		palSetPadMode(GPIOB, echoPin, PAL_MODE_INPUT);
	}
	extSetChannelModeI(&EXTD1, echoPin, &(ext->echoChannelConfig));
}

void UltrasonicSensor::startMeasurement() {
	if(!needsPWM)
	{
		palSetPad(GPIOB, triggerPin);
		chThdSleepMicroseconds(50);
		palClearPad(GPIOB, triggerPin);
	}
	else
	{
		//Enable PWM on TIM1_CH2 --> GPIO 1, PB14 with 40kHz
		pwm.start();
		chThdSleepMilliseconds(20);
		pwm.stop();
		ext->startTime[echoPin] = chVTGetSystemTime();
	}
}

/**
 * Returns the measured value in cm
 */
uint16_t UltrasonicSensor::getValue() {
	bool isHighFlank = this->ext->isHighFlank[echoPin];
	uint16_t startTime = this->ext->startTime[echoPin];
	uint16_t endTime = this->ext->endTime[echoPin];
	//uint16_t maxValue = 65535;

	//TODO Overflow
	if(!isHighFlank and startTime != 0 and endTime != 0) {
		if(endTime >= startTime)
			return ST2US(endTime - startTime)/DIVIDER;
		else
			return ST2US(65535 - startTime + endTime);
	}
	else {
		return 0;
	}
}
