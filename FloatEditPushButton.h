#pragma once
#include<qpushbutton.h>
#include"FloatEditDelegate.h"
#include<qdebug.h>
class FloatEditPushButton: public QPushButton
{
public:
	FloatEditPushButton(FloatEditDelegate*);
protected:
	FloatEditDelegate* responder;
	bool in = false;
	void leaveEvent(QEvent*);
	void enterEvent(QEnterEvent*);
};

