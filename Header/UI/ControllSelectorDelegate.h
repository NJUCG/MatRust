#pragma once
#include<iostream>
using namespace std;
class ControllSelectorDelegate
{
public:
	virtual void switchTo(string type) = 0;
};

