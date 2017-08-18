#include "../include/UART.h"
#include <stdio.h>

static SerialConfig sd5cfg = {
	115200 // Baudrate
};

UART::UART(uint8_t tx, uint8_t rx) {
	palSetPadMode(GPIOB, tx, PAL_MODE_ALTERNATE(4));
	palSetPadMode(GPIOB, rx, PAL_MODE_ALTERNATE(4));
	sdStart(&SD5, &sd5cfg);
}

void UART::write(uint16_t value1, uint16_t value2) {
	char strValueFirst[5];
	char strValueSecond[5];
	char strSizeFirst[1];
	char strSizeSecond[1];
	uint8_t lenOfFirstStr;
	uint8_t lenOfSecondStr;

	lenOfFirstStr = sprintf(strValueFirst, "%d", value1);
	lenOfSecondStr = sprintf(strValueSecond, "%d", value2);
	sprintf(strSizeFirst, "%d", lenOfFirstStr);
	sprintf(strSizeSecond, "%d", lenOfSecondStr);
	sdWrite(&SD5, (uint8_t *) &strSizeFirst, 1);
	sdWrite(&SD5, (uint8_t *) &strValueFirst, lenOfFirstStr);
	sdWrite(&SD5, (uint8_t *) &strSizeSecond, 1);
	sdWrite(&SD5, (uint8_t *) &strValueSecond, lenOfSecondStr);

//	sdWrite(&SD5, (uint8_t *) &valueOfFirst, sizeof(valueOfFirst));
//	sdWrite(&SD5, (uint8_t *) &valueOfSecond, sizeof(valueOfSecond));
}

void UART::writeChar(uint8_t data) {
	sdWrite(&SD5, (uint8_t *) &data, 1);
}
