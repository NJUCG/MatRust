#include "FloatMiddleButton.h"

FloatEditMiddleButton::FloatEditMiddleButton(FloatEditDelegate* responder)
{
	this->responder = responder;
}

void FloatEditMiddleButton::enterEvent(QEnterEvent* e)
{
	if (!in) {
		in = true;
		responder->middle_button_mouse_enter(e);
	}
}

void FloatEditMiddleButton::leaveEvent(QEvent* e)
{
	if (in) {
		in = false;
		responder->middle_button_mouse_leave(e);
	}
}

void FloatEditMiddleButton::mouseReleaseEvent(QMouseEvent* e)
{
	responder->middle_button_mouse_release(e);
}

void FloatEditMiddleButton::mouseMoveEvent(QMouseEvent* e)
{
	responder->middle_button_mouse_move(e);
}

void FloatEditMiddleButton::mousePressEvent(QMouseEvent* e)
{
	responder->middle_button_mouse_press(e);
}
