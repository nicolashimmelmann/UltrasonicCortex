#include <stdio.h>
#include <ch.hpp>
#include <hal.h>
#include <cstring>
using namespace chibios_rt;

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
	palSetPadMode(GPIOB, 14, PAL_MODE_INPUT_PULLUP);
}

uint16_t readValue() {
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

int main(void) {
	halInit();
	chSysInit();

	initUART();
	initSensor();
	sdStart(&SD5, &sd5cfg);

	while(1) {
		uint16_t duration = readValue();
		writeSensorDataUART(duration);
		chThdSleep(100);
	}
	return 0;
}
