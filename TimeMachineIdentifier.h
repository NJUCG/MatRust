#pragma once
#include"PipelineManager.h"
#include<qwidget.h>
#include"CssLoader.h"
#include<qpainter.h>
#include<sstream>
#include<iomanip>
#include<qtimer.h>
#include<vector>
#include<qstyleoption.h>
#include"TimeMachineResponder.h"
#include"EventAdapter.h"
using namespace std;
class TimeMachineIdentifier: public QWidget

{
	Q_OBJECT
public:
	TimeMachineIdentifier(float start_time = 0, float stop_time = 20, float step = 1);
	void register_responder(TimeMachineResponder*);
	~TimeMachineIdentifier();
	float current_time;
	QTimer* timer;

	void play();
	void pause();
	void clear();
protected:

	float start_time;
	float stop_time;
	float step;
	float width_dist;
	float dist_per_unit;

	int machine_margin = 5;
	int interval = -1;
	int tag_to_show = -1;

	void init();
	void addComponent();
	void paintEvent(QPaintEvent*) override;
	void resizeEvent(QResizeEvent*) override;
	string to_string(float);
	string to_string_by(float,int prec = 1);

	vector<TimeMachineResponder*> events;

	bool block_only = false;
signals:
	void time_exhausted();
};

