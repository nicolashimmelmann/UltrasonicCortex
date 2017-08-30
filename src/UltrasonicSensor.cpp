#include "include/UltrasonicSensor.h"


UltrasonicSensor::UltrasonicSensor(EXT * ext, short tP, short eP) {
	this->ext = ext;
	this->triggerPin = tP;
	this->echoPin = eP;
}

void UltrasonicSensor::init() {
	//Set pin modes for sensor
	palSetPadMode(GPIOB, triggerPin, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOB, echoPin, PAL_MODE_INPUT);
	extSetChannelModeI(&EXTD1, echoPin, &(ext->echoChannelConfig));
}

void UltrasonicSensor::startMeasurement() {
	palSetPad(GPIOB, triggerPin);
	chThdSleepMicroseconds(50);
	palClearPad(GPIOB, triggerPin);
}

/**
 * Returns the measured value in cm
 */
uint16_t UltrasonicSensor::getValue() {
	bool isHighFlank = this->ext->isHighFlank[echoPin];
	uint16_t startTime = this->ext->startTime[echoPin];
	uint16_t endTime = this->ext->endTime[echoPin];

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
