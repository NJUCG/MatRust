#pragma once
#include<qwidget.h>
#include"CssLoader.h"
#include"HeaderTab.h"
#include"StringHelper.h"

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

