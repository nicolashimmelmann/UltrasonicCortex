#ifndef INCLUDE_UART_H_
#define INCLUDE_UART_H_

#include <ch.hpp>
#include <hal.h>
#include <pal.h>

class UART {

public:
	UART(uint8_t tx, uint8_t rx);
	void write(uint16_t value1, uint16_t value2);

};

#endif /* INCLUDE_UART_H_ */
