#include <ch.hpp>
#include <hal.h>
#include <cstring>
using namespace chibios_rt;

#define USART_CR3_ENABLE_RTS (1 << 8)
#define USART_CR3_ENABLE_CTS (1 << 9)

//Configure Serial with SerialConfig Struct
static SerialConfig sd2cfg = {
	9600, // Baudrate
	0,
	0,
	0//USART_CR3_ENABLE_RTS | USART_CR3_ENABLE_CTS
};

// https://github.com/fcayci/STM32F4-ChibiOS/blob/master/serial/main.c
// http://www.chibios.com/forum/viewtopic.php?t=1341
// MCU_USART2_CTS = PA0
// MCU_USART2_RTS = PA1
// MCU_USART2_TX = PA2
// MCU_USART2_RX = PA3
int main(void) {
	halInit();
	chSysInit();

	//Init UART2 on GPIOs. Alternate Function AF1 (=USART2) needs to be activated
	// (Cortex M0 Datasheet, p.33; Ref.Manual p. 140)
	palSetPadMode(GPIOA, 0, PAL_MODE_ALTERNATE(1)); //CTS
	palSetPadMode(GPIOA, 1, PAL_MODE_ALTERNATE(1)); //RTS
	palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(1)); //TX
	palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(1)); //RX

	//Start SerialDriver. sdInit() is implicitly called by halInit() above
	sdStart(&SD2, &sd2cfg);
	//Buffer for the characters
	//uint8_t sendBuf = 4;
	//char sendBuf[] = {'A','T'};
	uint32_t x = 0;
	//char recBuf[2];
	while(1)
	{
		sdWrite(&SD2, (uint8_t *) &x, 4);
		//sdRead(&SD2, (uint8_t *) recBuf, 2);
		chThdSleepMilliseconds(1000);
		x+=1;
	}

	return 0;
}

