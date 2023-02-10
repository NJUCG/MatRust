#pragma once
enum ControlTabParam
{
	ENV_OC
};
class ControlTabDelegate
{
public:
	ControlTabDelegate();
	virtual void value_changed(ControlTabParam arg, float newValue);
};

