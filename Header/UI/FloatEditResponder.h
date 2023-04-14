#pragma once
#include<string>
using namespace std;

class FloatEditResponder
{
public:
	virtual void on_value_changed(string name, float new_value);
};

