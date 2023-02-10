#pragma once
#include<string>
using namespace std;
class EventResponder
{
public:
	EventResponder();
	virtual void on_trigger(string);
};

