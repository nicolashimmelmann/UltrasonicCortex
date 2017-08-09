#include <stdio.h>
#include <ch.hpp>
#include <hal.h>
#include <pal.h>
#include <cstring>
#include <stm32f030xc.h>

#include "include/Stepper.h"
#include "include/Bluetooth.h"
#include "include/UltrasonicSensor.h"

using namespace chibios_rt;

bool is_high_sensor1 = false;
bool is_high_sensor2 = false;
systime_t start1;
systime_t end1;
systime_t start2;
systime_t end2;

static bool IS_ACTIVE = false;
char CMD_START = 'S';


static SerialConfig sd5cfg = {
	38400 // Baudrate
};

void initUART() {
	palSetPadMode(GPIOB, 3, PAL_MODE_ALTERNATE(4));
	palSetPadMode(GPIOB, 4, PAL_MODE_ALTERNATE(4));
}

void writeSensorDataUART(uint16_t duration) {
	char _duration[5];
	char _size_duration[1];

	int size_duration = sprintf(_duration, "%d", duration);
	sprintf(_size_duration, "%d", size_duration);

	sdWrite(&SD5, (uint8_t *) &_size_duration, 1);
	sdWrite(&SD5, (uint8_t *) &_duration, size_duration);
}

void initSensor() {
	//GPIO_0 (15) Trigger
	//GPIO_1 (14) Echo
	palSetPadMode(GPIOB, 15, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOB, 14, PAL_MODE_INPUT);

	//GPIO_2 (6) Trigger
	//GPIO_3 (5) Echo
	palSetPadMode(GPIOB, 6, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOB, 5, PAL_MODE_INPUT);
}

uint32_t readValue() {
	systime_t start;
	systime_t duration;

	while(1) {
		palSetPad(GPIOB, 15);
		chThdSleepMicroseconds(10);
		palClearPad(GPIOB, 15);

		while(1) {
			if(palReadPad(GPIOB, 14)) {
				start = chVTGetSystemTime();
				break;
			}
		}
		while(palReadPad(GPIOB, 14));
		// double schallgeschwindigkeit = 331.5 + 0.6 * 30;
		// double entfernung = (schallgeschwindigkeit * duration)/20000;
		return duration = ST2US(chVTGetSystemTime() - start);
	}
}


static void sensor1(EXTDriver *extp, expchannel_t channel) {
	(void)extp;
	(void)channel;
	chSysLockFromISR();
	if(!is_high_sensor1)
		start1 = chVTGetSystemTimeX();
	else
		end1 = chVTGetSystemTimeX();
	is_high_sensor1 = !is_high_sensor1;
	chSysUnlockFromISR();
}

static void sensor2(EXTDriver *extp, expchannel_t channel) {
	(void)extp;
	(void)channel;
	chSysLockFromISR();
	if(!is_high_sensor2)
		start2 = chVTGetSystemTimeX();
	else
		end2 = chVTGetSystemTimeX();
	is_high_sensor2 = !is_high_sensor2;
	chSysUnlockFromISR();
}

static const EXTConfig extcfg = {
  {
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOB, sensor2},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOB, sensor1},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
//    {EXT_CH_MODE_DISABLED, NULL},
//    {EXT_CH_MODE_DISABLED, NULL},
//    {EXT_CH_MODE_DISABLED, NULL}
  }
};

void waitForStartCommand(Bluetooth * bt) {
	//Do nothing until start is received
	char cmd = '0';
	while(cmd != CMD_START) {
		cmd = (*bt).readCommand(false);
	}
	IS_ACTIVE = true;
}


int main(void) {
	halInit();
	chSysInit();

	initUART();
	initSensor();
	Stepper stepper(64);
	Bluetooth bt;

	sdStart(&SD5, &sd5cfg);

	waitForStartCommand(&bt);

	//Enable interrupt channels for the two sensors
	extStart(&EXTD1, &extcfg);
	extChannelEnable(&EXTD1, 5);
	extChannelEnable(&EXTD1, 14);

	//Start measurements for each sensor
	palSetPad(GPIOB, 15);
	palSetPad(GPIOB, 6);
	chThdSleepMicroseconds(100);
	palClearPad(GPIOB, 15);
	palClearPad(GPIOB, 6);

	int n = 0;
	while(1) {
		if(IS_ACTIVE)
		{
			uint32_t data = readValue();
			//writeSensorDataUART(data);
			bt.send(data);

			//Turn the motor
			stepper.tick();
			++n;

			//Read stop command from bluetooth with timeout to not block too long
			char cmd = bt.readCommand(true);
			if(cmd == CMD_START) {
				IS_ACTIVE = false;
			}
		}
		else
		{
			waitForStartCommand(&bt);
		}
	}
	return 0;
}
