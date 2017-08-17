/*
 * UltrasonicSensor.h
 *
 *  Created on: Aug 8, 2017
 *      Author: felix
 */
#ifndef ULTRASONICSENSOR_H_
#define ULTRASONICSENSOR_H_

#include "include/EXT.h"
#include "include/PWM.h"

class UltrasonicSensor {
public:
	UltrasonicSensor(EXT * ext, short triggerPin, short echoPin, bool needsPWM);
	void startMeasurement();
	uint16_t getValue();
	void init();

private:
	const uint16_t DIVIDER = 58;
	short echoPin;
	short triggerPin;
	bool needsPWM;
	PWM pwm;
	EXT * ext;
};

#endif /* ULTRASONICSENSOR_H_ */
