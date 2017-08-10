/*
 * UltrasonicSensor.h
 *
 *  Created on: Aug 8, 2017
 *      Author: felix
 */
#include <hal.h>

#ifndef ULTRASONICSENSOR_H_
#define ULTRASONICSENSOR_H_

class UltrasonicSensor {
public:
	UltrasonicSensor(short triggerPin, short echoPin);
	void startMeasurement();
	uint16_t getValue();

private:
	static void interruptHandler(EXTDriver *extp, expchannel_t channel);

	const uint16_t DIVIDER = 58;

	static bool isHighFlank[20];
	static systime_t startTime[20];
	static systime_t endTime[20];
	short echoPin;
	short triggerPin;
	static bool extStarted;

	static const EXTChannelConfig echoChannelConfig;
	static EXTConfig extcfg;

};

#endif /* ULTRASONICSENSOR_H_ */
