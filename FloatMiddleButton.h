#pragma once
#include<qpushbutton.h>
#include"FloatEditDelegate.h"
class FloatEditMiddleButton: public QPushButton
{
public:
	FloatEditMiddleButton(FloatEditDelegate*);
protected:
	FloatEditDelegate* responder;
	bool in = false;
	void enterEvent(QEnterEvent*);
	void leaveEvent(QEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void mousePressEvent(QMouseEvent*);
};

