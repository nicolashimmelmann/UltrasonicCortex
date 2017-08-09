#ifndef BLUETOOTH_H__
#define BLUETOOTH_H__

#include <ch.hpp>

class Bluetooth {

public:
	Bluetooth();

	void send(uint32_t data);
	char readCommand(bool timeout);

private:
};

#endif

