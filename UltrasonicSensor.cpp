/*
 * UltrasonicSensor.cpp
 *
 *  Created on: Aug 8, 2017
 *      Author: felix
 */

#include "include/UltrasonicSensor.h"

bool UltrasonicSensor::extStarted = false;
bool UltrasonicSensor::isHighFlank[] = {false, false, false, false, false,
									    false, false, false, false, false,
									    false, false, false, false, false,
									    false, false, false, false, false};
systime_t UltrasonicSensor::startTime[20] = {0,0,0,0,0,
											 0,0,0,0,0,
											 0,0,0,0,0,
											 0,0,0,0,0};
systime_t UltrasonicSensor::endTime[20] = {0,0,0,0,0,
										   0,0,0,0,0,
										   0,0,0,0,0,
										   0,0,0,0,0};
const EXTChannelConfig UltrasonicSensor::echoChannelConfig = {EXT_CH_MODE_BOTH_EDGES | EXT_MODE_GPIOB, interruptHandler};
EXTConfig UltrasonicSensor::extcfg = {
  {
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
  }
};


UltrasonicSensor::UltrasonicSensor(short tP, short eP) {
	this->triggerPin = tP;
	this->echoPin = eP;
	palSetPadMode(GPIOB, triggerPin, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOB, echoPin, PAL_MODE_INPUT);

	if(!extStarted) {
		extStart(&EXTD1, &extcfg);
		extStarted = !extStarted;
	}
	extSetChannelModeI(&EXTD1, echoPin, &echoChannelConfig);
}

void UltrasonicSensor::interruptHandler(EXTDriver *extp, expchannel_t channel) {
	(void)extp;
	(void)channel;
	chSysLockFromISR();
	if(!isHighFlank[channel])
		startTime[channel] = chVTGetSystemTimeX();
	else {
		endTime[channel] = chVTGetSystemTimeX();
	}
	isHighFlank[channel] = !isHighFlank[channel];
	chSysUnlockFromISR();
}

void UltrasonicSensor::startMeasurement() {
	palSetPad(GPIOB, triggerPin);
	chThdSleepMicroseconds(10);
	palClearPad(GPIOB, triggerPin);
}

/**
 * Returns the measured value in cm
 */
uint16_t UltrasonicSensor::getValue() {
	bool isHighFlank = UltrasonicSensor::isHighFlank[echoPin];
	uint16_t startTime = UltrasonicSensor::startTime[echoPin];
	uint16_t endTime = UltrasonicSensor::endTime[echoPin];
	uint16_t maxValue = 65535;

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
