#pragma once
#include<qlineedit.h>
#include"FloatEditDelegate.h"
class FloatEditLineEdit: public QLineEdit
{
public:
	FloatEditLineEdit(FloatEditDelegate*,int);
	int w;
protected:
	int h;

	FloatEditDelegate* responder;
	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;
};

