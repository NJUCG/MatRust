#pragma once
class TimeMachineResponder
{
public:
	int w;
	bool pause = false;
	// ÏûºÄÖ¡
	virtual void frame_consumer(float);
	virtual void update_current(float);
};

