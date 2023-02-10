#pragma once
#include<qwidget.h>
#include"CssLoader.h"
class Header:public QWidget
{
public:
	Header();
protected:
	void init();
	void addComponent();
};

