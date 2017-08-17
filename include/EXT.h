/*
 * EXT.h
 *
 *  Created on: 17.08.2017
 *      Author: nicolas
 */

#ifndef INCLUDE_EXT_H_
#define INCLUDE_EXT_H_

#include <hal.h>

class EXT {

public:
	EXT();

	//Determines for each EXT channel if a high flank was registered
	static bool isHighFlank[20];

	//Start time for the respective EXT channel
	static systime_t startTime[20];

	//End time for the respective EXT channel
	static systime_t endTime[20];

	//Handles rising and falling edges on each EXT channel
	static void interruptHandler(EXTDriver *extp, expchannel_t channel);

	const EXTChannelConfig echoChannelConfig = {EXT_CH_MODE_BOTH_EDGES | EXT_MODE_GPIOB, interruptHandler};

	//The ext config
	EXTConfig extcfg = {
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

#endif /* INCLUDE_EXT_H_ */
