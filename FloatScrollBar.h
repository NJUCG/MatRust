#pragma once
#include<qevent.h>
#include<qwidget.h>
#include<qprogressbar.h>
#include<string>
#include"CssLoader.h"
#include"FloatProgressBarResponder.h"
#include<qlayout.h>
#include<qscrollbar.h>
#include<qslider.h>
#include<qlabel.h>
#include<sstream>
#include<iomanip>
#include"UIModel.h"
#include"NodeData.h"
using namespace std;
class FloatSliderBar:public QSlider, public NodeDataResponder
{
public:
	FloatSliderBar(FloatSliderBarResponder* responder,QString title, float min, float max, float delta = 100);
	float d_value();
	void on_streamed(void*,int) override;
	void value_changed(void*,int) override;
	void on_cutoff(void*,int) override;
	void set_node_data(NodeData<float>*);
protected:
	NodeData<float>* node_data = nullptr;
	FloatSliderBarResponder* responder;
	QLabel* name_label, *value_label;
	QString title;
	float delta;
	bool streamed = false;
	void init();
	void addComponent();
	void onValueChanged(int value);
};

