#include "FloatEditLabel.h"

FloatEditLabel::FloatEditLabel(FloatEditDelegate* responder)
{
	this->responder = responder;
}

void FloatEditLabel::enterEvent(QEnterEvent* e)
{
	responder->label_mouse_enter(e);
}
