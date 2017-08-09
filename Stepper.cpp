#include "include/Stepper.h"
#include <hal.h>
#include <pal.h>
#include <stm32f030xc.h>

/**
 * Initializes the pins GPIO 4, 5, 6 and 7 to control a stepper motor.
 *
 * @param max_steps The number of steps after which to turn into the opposite direction.
 * @param steps_per_tick The number of steps to take for one tick command.
 */
Stepper::Stepper(short max_ticks) {
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

	//Number of steps after which to turn around
	MAX_STEPS = STEPS_PER_REVOLUTION / 2;
	//Calculate the steps necessary for a tick.
	stepsPerTick = MAX_STEPS / max_ticks;
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

void Stepper::tick() {
	makeNSteps(stepsPerTick);
}

/**
 * Make the next step.
 */
void Stepper::makeStep() {
	if(forward)
	{
		//Go forward by increasing n from 0 to 3
		//and making a step each time
		++currentStep;
		makeStepForward();
	} else {
		//Go backward by decreasing n from 3 to 0
		//and making a step each time
		--currentStep;
		makeStepBackward();
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

void Stepper::makeNSteps(short n) {
	while(n--)
	{
		makeStep();
	}
}

void Stepper::makeStepForward() {
	short n = 0;
	while(n < 4)
	{
		step(n);
		++n;
	}
}

void Stepper::makeStepBackward() {
	short n = 4;
	while(n > 0)
	{
		step(n-1);
		--n;
	}
}

/**
 * Change the phases of the motor using setPins() above.
 */
void Stepper::step(short n) {
	switch (n)
	{
		case 0:  // 0011
			setPins(false, false, true, true);
			break;
		case 1:  // 1001
			setPins(true, false, false, true);
			break;
		case 2:  //1100
			setPins(true, true, false, false);
			break;
		case 3:  //0110
			setPins(false, true, true, false);
			break;
	}
	chThdSleep(60);
}


