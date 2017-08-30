#ifndef ULTRASONICSENSOR_H_
#define ULTRASONICSENSOR_H_

#include "include/EXT.h"

class UltrasonicSensor {
public:
	UltrasonicSensor(EXT * ext, short triggerPin, short echoPin);
	void startMeasurement();
	uint16_t getValue();
	void init();

private:
	const uint16_t DIVIDER = 58;
	short echoPin;
	short triggerPin;
	EXT * ext;
};

#endif /* ULTRASONICSENSOR_H_ */
