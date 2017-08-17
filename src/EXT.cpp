#include "include/EXT.h"

bool EXT::isHighFlank[] = {false, false, false, false, false,
		  false, false, false, false, false,
		  false, false, false, false, false,
		  false, false, false, false, false};

systime_t EXT::startTime[]  = {0,0,0,0,0,
		   0,0,0,0,0,
		   0,0,0,0,0,
		   0,0,0,0,0};

systime_t EXT::endTime[] = {0,0,0,0,0,
		 0,0,0,0,0,
		 0,0,0,0,0,
		 0,0,0,0,0};

EXT::EXT() {
	extStart(&EXTD1, &extcfg);
}

/**
 * Interrupt handler for handling the start and end of the signal
 */
void EXT::interruptHandler(EXTDriver *extp, expchannel_t channel) {
	(void)extp;
	(void)channel;

	chSysLockFromISR();

	if(!isHighFlank[channel])
	{
		//Save time on high flank
		startTime[channel] = chVTGetSystemTimeX();
	}
	else
	{
		//Save time on low flank
		endTime[channel] = chVTGetSystemTimeX();
	}

	isHighFlank[channel] = !isHighFlank[channel];

	palTogglePad(GPIOB, 1);

	chSysUnlockFromISR();
}
