#ifndef STEPPER_H__
#define STEPPER_H__

class Stepper {

public:
	Stepper();

	void makeStep();

	void makeNSteps(short n);

private:
	const int MAX_STEPS = 128;

	void step(short n);
	void setPins(bool, bool, bool, bool);

	int currentStep;
	bool forward;

};


#endif
