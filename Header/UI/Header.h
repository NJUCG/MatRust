#pragma once
#include"CssLoader.h"
#include"HeaderTab.h"
#include"StringHelper.h"
#include<qwidget.h>
#include<qpushbutton.h>

class Header:public QWidget
{
public:
	Header();
protected:
	QHBoxLayout* top_layout = nullptr;

	void init();
	void addComponent();
	void add_tab(HeaderTab*);
};

