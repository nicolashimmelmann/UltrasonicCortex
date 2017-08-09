#include "include/Bluetooth.h"
#include <ch.hpp>
#include <hal.h>
using namespace chibios_rt;

static SerialConfig sd2cfg = {
	9600, // Baudrate
	0,
	0,
	0 //USART_CR3_ENABLE_RTS | USART_CR3_ENABLE_CTS
};

// https://github.com/fcayci/STM32F4-ChibiOS/blob/master/serial/main.c
// http://www.chibios.com/forum/viewtopic.php?t=1341
// MCU_USART2_CTS = PA0
// MCU_USART2_RTS = PA1
// MCU_USART2_TX = PA2
// MCU_USART2_RX = PA3

Bluetooth::Bluetooth() {
	//Init UART2 on GPIOs. Alternate Function AF1 (=USART2) needs to be activated
	// (Cortex M0 Datasheet, p.33; Ref.Manual p. 140)
	palSetPadMode(GPIOA, 0, PAL_MODE_ALTERNATE(1)); //CTS
	palSetPadMode(GPIOA, 1, PAL_MODE_ALTERNATE(1)); //RTS
	palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(1)); //TX
	palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(1)); //RX

	//Start SerialDriver. sdInit() is implicitly called by halInit() above
	sdStart(&SD2, &sd2cfg);
}

void Bluetooth::send(uint16_t data1, uint16_t data2) {
	//Buffer for the characters
	uint16_t dataArray[] = {data1, data2};
	sdWrite(&SD2, (uint8_t *) &dataArray, 4);
}

/**
 * Reads a command from bluetooth and returns it.
 */
char Bluetooth::readCommand(bool timeout) {
	char cmd = '0';
	if(!timeout)
	{
		//Block until something is received
		sdRead(&SD2, (uint8_t *) &cmd, 1);
	}
	else
	{
		//Timeout after 10ms
		sdReadTimeout(&SD2, (uint8_t *) &cmd, 1, 10);
	}
	return cmd;
}
