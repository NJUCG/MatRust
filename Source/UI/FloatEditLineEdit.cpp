#include "FloatEditLineEdit.h"

FloatEditLineEdit::FloatEditLineEdit(FloatEditDelegate* responder, int h)
{
	this->responder = responder;
	this->h = h;
}


QSize FloatEditLineEdit::sizeHint() const
{
	return QSize(103, 40);
}

QSize FloatEditLineEdit::minimumSizeHint() const
{
	return QSize(103, 40);
}
