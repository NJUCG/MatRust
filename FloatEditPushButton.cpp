#include "FloatEditPushButton.h"

FloatEditPushButton::FloatEditPushButton(FloatEditDelegate* responder)
{
	this->responder = responder;
}

void FloatEditPushButton::leaveEvent(QEvent* e)
{
	if (in) {
		in = false;
		responder->pushbutton_mouse_leave(e);
	}
}

void FloatEditPushButton::enterEvent(QEnterEvent* e)
{

	if (!in) {
		in = true;
		responder->pushbutton_mouse_enter(e);
	}
}
