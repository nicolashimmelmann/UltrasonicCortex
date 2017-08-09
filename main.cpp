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
static bool IS_ACTIVE = false;
char CMD_START = 'S';


static SerialConfig sd5cfg = {
	38400 // Baudrate
};

void initUART() {
	palSetPadMode(GPIOB, 3, PAL_MODE_ALTERNATE(4));
	palSetPadMode(GPIOB, 4, PAL_MODE_ALTERNATE(4));
}

void writeSensorDataUART(uint16_t duration, char sensor) {
	char _duration[5];
	char _size_duration[1];

	int size_duration = sprintf(_duration, "%d", duration);
	sprintf(_size_duration, "%d", size_duration);
	//sdWrite(&SD5, (uint8_t *) &sensor, 1);
	sdWrite(&SD5, (uint8_t *) &sensor, 1);
	sdWrite(&SD5, (uint8_t *) &_size_duration, 1);
	sdWrite(&SD5, (uint8_t *) &_duration, size_duration);
}


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

	// UART for testing with python script on pi
	//initUART();
	//sdStart(&SD5, &sd5cfg);

	Stepper stepper(64);
	Bluetooth bt;
	UltrasonicSensor sensor1(15, 14);
	UltrasonicSensor sensor2(6, 5);


	waitForStartCommand(&bt);


	int n = 0;
	while(1) {
		sensor1.startMeasurement();
		sensor2.startMeasurement();
		chThdSleepMilliseconds(100);
		uint16_t duration1 = sensor1.getValue();
		uint16_t duration2 = sensor2.getValue();

		/*if(duration1 > 0)
			writeSensorDataUART(duration1, '1');
		if(duration2 > 0)
			writeSensorDataUART(duration2, '2');
		chThdSleep(3000);*/

		if(IS_ACTIVE)
		{
			//writeSensorDataUART(data);
			bt.send(duration1, duration2);

			//Turn the motor
			stepper.tick();
			++n;

			//Read stop command from Bluetooth with timeout to not block too long
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
