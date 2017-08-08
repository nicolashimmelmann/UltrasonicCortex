#include "include/Stepper.h"
#include <hal.h>
#include <pal.h>
#include <stm32f030xc.h>

/**
 * Initializes the pins GPIO 4, 5, 6 and 7 to control a stepper motor.
 */
Stepper::Stepper() {
	//GIPOs 4, 5, 6, 7
	//GPIO4 = PC13
	//GPIO5 = PB7
	//GPIO6 = PF0
	//GPIO7 = PC14
	palSetPadMode(GPIOC, 13, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOB, 7, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOF, 0, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode(GPIOC, 14, PAL_MODE_OUTPUT_PUSHPULL);

	//Initialize direction
	currentStep = 0;
	forward = true;
}

/**
 * Set the pins 1 to 4 (a to d) as desired.
 */
void Stepper::setPins(bool a, bool b, bool c, bool d) {
	if(a) palSetPad(GPIOC, 13); else palClearPad(GPIOC, 13);
	if(b) palSetPad(GPIOB,  7); else palClearPad(GPIOB,  7);
	if(c) palSetPad(GPIOF,  0); else palClearPad(GPIOF,  0);
	if(d) palSetPad(GPIOC, 14); else palClearPad(GPIOC, 14);
}


void Stepper::makeNSteps(short n) {
	while(n--)
	{
		makeStep();
	}
}

/**
 * Make the next step-
 */
void Stepper::makeStep() {
	if(forward)
	{
		//Go forward by increasing n from 0 to 3
		//and making a step each time
		++currentStep;
		short n = 0;
		while(n < 4)
		{
			step(n);
			++n;
		}
	} else {
		//Go backward by decreasing n from 3 to 0
		//and making a step each time
		--currentStep;
		short n = 3;
		while(n > 0)
		{
			step(n);
			--n;
		}
	}

	//Change direction if necessary
	if(currentStep >= MAX_STEPS)
	{
		forward = false;
		currentStep = MAX_STEPS - 1;
	}
	else if(currentStep <= 0)
	{
		forward = true;
		currentStep = 0;
	}
}

/**
 * Change the phases of the motor using setPins() above.
 */
void Stepper::step(short n) {
	switch (n)
	{
		case 0:  // 1010
			setPins(true, false, true, false);
			break;
		case 1:  // 0110
			setPins(false, true, true, false);
			break;
		case 2:  //0101
			setPins(false, true, false, true);
			break;
		case 3:  //1001
			setPins(true, false, false, true);
			break;
	}
	chThdSleep(50);
}


