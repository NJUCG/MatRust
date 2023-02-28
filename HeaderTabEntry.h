#pragma once
#include<qwidget.h>
#include<qlabel.h>
#include<qlayout.h>
#include<iostream>
#include<string>
#include"UIModel.h"
#include"EventAdapter.h"
using namespace std;

class HeaderTabEntry: public QWidget
{
public:
	HeaderTabEntry();
	HeaderTabEntry(string);
	void set_event(string);
protected:
	void init();
	void addComponent();
};

