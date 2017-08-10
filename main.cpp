#include <stdio.h>
#include <ch.hpp>
#include <hal.h>
#include <pal.h>
#include <cstring>
#include <stm32f030xc.h>

#include "include/Stepper.h"
#include "include/Bluetooth.h"
#include "include/UltrasonicSensor.h"
#include "include/Utils.h"

using namespace chibios_rt;

static bool IS_ACTIVE = false;
const char CMD_START = 'S';
const short FILTER_SIZE = 3;

static uint16_t data1[FILTER_SIZE];
static uint16_t data2[FILTER_SIZE];

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

	Stepper stepper(32);
	Bluetooth bt;
	UltrasonicSensor sensor1(15, 14);
	UltrasonicSensor sensor2(6, 5);


	waitForStartCommand(&bt);


	int n = 0;
	while(1)
	{
		if(IS_ACTIVE)
		{
			for(int i = 0; i<FILTER_SIZE; ++i)
			{
				sensor1.startMeasurement();
				sensor2.startMeasurement();
				chThdSleepMilliseconds(50);
				data1[i] = sensor1.getValue();
				data2[i] = sensor2.getValue();
			}
			Utils::sort(data1, FILTER_SIZE);
			Utils::sort(data2, FILTER_SIZE);

			uint16_t val1 = data1[FILTER_SIZE/2];
			uint16_t val2 = data2[FILTER_SIZE/2];

			//writeSensorDataUART(data1);
			bt.send(val1, val2);

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
