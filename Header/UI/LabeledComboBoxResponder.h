#pragma once
#include<string>
#include<qstring.h>
using namespace std;

class LabeledComboBoxResponder
{
public:
	virtual void value_changed(QString, QString);
};

