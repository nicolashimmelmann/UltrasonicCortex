#include "include/PWM.h"

//The informtion to use PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH
//we found on http://www.chibios.com/forum/viewtopic.php?f=2&t=344
//This is necessary because on PB14, the output is TIM1_CH2N (inverted).
//To use this, STM32_PWM_USE_ADVANCED must be enabled in mcuconf.h
static PWMConfig pwmcfgNormal = {
   40000, //Timer frequency (1MHz) --> PWM_freq = Timer_freq / ticks
   1, //PWM Ticks per clockcycle
   NULL,
   {
	  {PWM_OUTPUT_DISABLED, NULL},
	  {PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH, NULL},
	  {PWM_OUTPUT_DISABLED, NULL},
	  {PWM_OUTPUT_DISABLED, NULL}
   },
   //HW dependent part.
   0,
   0  //STM32_PWM_USE_ADVANCED is enabled which adds additional field
};

/**
* Initialize the timer and GPIO registers to generate and output
* a PWM signal with a duty cycle of 50 %.
* @param frequency the PWM frequency in Hz
*/
void PWM::init(uint32_t frequency) {
	//PWM period ticks = PWM Timer frequency / wanted PWM frequency
	pwmcfgNormal.period = pwmcfgNormal.frequency / frequency;

	//Enable the alternate function to output the inverted Timer Channel on the Pin
	palSetPadMode(GPIOB, 14, PAL_MODE_ALTERNATE(2)); //PB14 = GPIO 1, TIM1_CH2N

	//Start PWM
	pwmStart(&PWMD1, &pwmcfgNormal);
}

/**
* Modify the duty cycle of the PWM signal.
* @param dc the new duty cycle in Q0.16 fixedpoint format
*/
void PWM::setDutyCycle(uint16_t dc) {
	//A Q0.16 format number can be expressed as a fraction, where the denominator is
	//the number of bits all set to 1. The numerator is simply the number itself.
	//0xFFFF is the denominator when calculating the fraction from a Q0.16 number
	pwmEnableChannel(&PWMD1, 1, PWM_FRACTION_TO_WIDTH(&PWMD1, 0xFFFF, dc));
}

void PWM::start() {
	//Enable PWM for channel 2 (zero-based index) with 50% duty cycle
	pwmEnableChannel(&PWMD1, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
}

void PWM::stop() {
	pwmDisableChannel(&PWMD1, 1);
}




