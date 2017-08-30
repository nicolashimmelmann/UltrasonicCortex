#include <stdio.h>
#include <ch.hpp>
#include <hal.h>
#include <pal.h>
#include <stm32f030xc.h>

#include "include/EXT.h"
#include "include/Stepper.h"
#include "include/Bluetooth.h"
#include "include/UART.h"
#include "include/UltrasonicSensor.h"
#include "include/Utils.h"

using namespace chibios_rt;

static bool IS_ACTIVE = false;
const char CMD_START = 'S';
const short FILTER_SIZE = 3;

static uint16_t data1[FILTER_SIZE];
static uint16_t data2[FILTER_SIZE];

bool waitForCommand(Bluetooth * bt, UART * uart, Stepper * step) {
	//Do nothing until start is received
	char cmd = '0';
	//Wait for start or reset command
	while(cmd == '0') {
		cmd = (*bt).readCommand(false);
	}
	switch(cmd) {
		case 'S':
			IS_ACTIVE = true;
			break;

		case 'R':
			IS_ACTIVE = false;
			//Clear measurement arrays
			for(int i = 0; i<FILTER_SIZE; ++i)
			{
				data1[i] = 0;
				data2[i] = 0;
			}
			//Send reset command to raspberry pi animation
			(*uart).writeChar('R');
			//Reset stepper
			(*step).reset();
			return true;

		default:
			return false;
	}
	IS_ACTIVE = true;
}

int main(void) {
	halInit();
	chSysInit();

	EXT ext;
	UART uart(3, 4);
	Stepper stepper(32);
	Bluetooth bt;
	UltrasonicSensor sensor1(&ext, 15, 14); //HC-SR04, GPIOs 0 and 1
	sensor1.init();
	UltrasonicSensor sensor2(&ext, 6, 5); //HC-SR04, GPIOs 2 and 3
	sensor2.init();

	//Wait for start command
	waitForCommand(&bt, &uart, &stepper);

	while(1)
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

		//Read stop command from Bluetooth if available
		//(with timeout to not block)
		char cmd = bt.readCommand(true);
		if(cmd == CMD_START) {
			IS_ACTIVE = false;
		}

		//Wait for start or reset command
		if(!IS_ACTIVE) {
			bool reset = waitForCommand(&bt, &uart, &stepper);
			//Check if reset command was received
			if(reset) {
				continue;
			}
		}

		// Send both sensor measurement
		uart.write(val1, val2);
		bt.send(val1, val2);

		//Turn the motor
		stepper.tick();

	}
	return 0;
}
