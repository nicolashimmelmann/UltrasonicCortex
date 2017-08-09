#ifndef STEPPER_H__
#define STEPPER_H__

class Stepper {

public:
	Stepper(short max_ticks);

	void tick();

	void makeStep();
	void makeNSteps(short n);
	void makeStepForward();
	void makeStepBackward();

private:
	short STEPS_PER_REVOLUTION = 512;
	short MAX_STEPS = 10;
	short stepsPerTick = 1;

	void step(short n);
	void setPins(bool, bool, bool, bool);

	int currentStep;
	bool forward;

};


#endif
