#pragma once
#include"TimeMachineIdentifier.h"
#include<qwidget.h>
#include"CssLoader.h"
#include<qboxlayout.h>
#include<qpushbutton.h>
#include<qlabel.h>
#include"StringHelper.h"

class PlayerWidget: public QWidget
{
public:
	PlayerWidget();
	~PlayerWidget();
	void register_timer(TimeMachineResponder*);
protected:
	QVBoxLayout* top_layout;
	QHBoxLayout* header_layout;
	QVBoxLayout* body_layout;

	TimeMachineIdentifier* time_machine;

	QPushButton* play_and_pause_button;
	QIcon play_icon;
	QIcon pause_icon;
	
	QPushButton* replay_button = new QPushButton();

	void init();
	void addComponent();
	void header();
	void body();
	void player();
	void on_time_exhausted();
};

