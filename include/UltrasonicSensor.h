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
	uint16_t getValue();
private:
	void interruptHandler(EXTDriver *extp, expchannel_t channel);
	static bool extStarted = false;
	bool isHighFlank = false;
	bool finishedMeasurment = false;
	systime_t startTime = 0;
	systime_t endTime = 0;

	static const EXTChannelConfig echoChannelConfig = {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOB, interruptHandler};
	static const EXTConfig extcfg = {
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

};

#endif /* ULTRASONICSENSOR_H_ */
