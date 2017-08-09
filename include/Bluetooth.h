#ifndef BLUETOOTH_H__
#define BLUETOOTH_H__

#include <ch.hpp>

class Bluetooth {

public:
	Bluetooth();

	void send(uint16_t data1, uint16_t data2);
	char readCommand(bool timeout);

private:
};

#endif

