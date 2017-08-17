#ifndef PWM_H__
#define PWM_H__


#include <hal.h>
#include <stm32f030xc.h>

class PWM {

public:
	/**
	* Initialize the timer and GPIO registers to generate and output
	* a PWM signal with a duty cycle of 50 %.
	* @param frequency the PWM frequency in Hz
	*/
	void init(uint32_t frequency);

	/**
	* Modify the duty cycle of the PWM signal.
	* @param dc the new duty cycle in Q0.16 fixedpoint format
	*/
	void setDutyCycle(uint16_t dc);

	void start();
	void stop();

};

#endif
