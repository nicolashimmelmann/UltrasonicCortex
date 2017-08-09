/*
 * UltrasonicSensor.cpp
 *
 *  Created on: Aug 8, 2017
 *      Author: felix
 */

#include "include/UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(short triggerPin, short echoPin) {
	palSetPadMode(GPIOB, triggerPin, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOB, echoPin, PAL_MODE_INPUT);

	if(!extStarted) {
		extStart(&EXTD1, &extcfg);
		extStarted = !extStarted;
	}

	extSetChannelMode(&EXTD1, echoPin, &echoChannelConfig);
	extChannelEnable(&EXTD1, echoPin);
}

void UltrasonicSensor::interruptHandler(EXTDriver *extp, expchannel_t channel) {
	(void)extp;
	(void)channel;
	chSysLockFromISR();
	if(!isHighFlank)
		startTime = chVTGetSystemTimeX();
	else {
		endTime = chVTGetSystemTimeX();
		finishedMeasurment = true;
	}
	isHighFlank = !isHighFlank;
	chSysUnlockFromISR();
}

uint16_t UltrasonicSensor::getValue() {
	uint16_t result;
	uint16_t maxValueUint = 65536;

	if(isHighFlank and startTime != 0 and endTime != 0) {
		if(startTime < endTime)
			result = ST2US(endTime - startTime);
		else
			result = ST2US(maxValueUint - startTime + endTime);
	}

	// Check if measurement failed
	if(result != 38000)
		return result;
	else
		return 0;
}
