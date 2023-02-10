#pragma once
#include"FloatEditDelegate.h"
#include<qlabel.h>
class FloatEditLabel: public QLabel
{
public:
	FloatEditLabel(FloatEditDelegate* responder);
protected:
	FloatEditDelegate* responder;
	void enterEvent(QEnterEvent*);
};

