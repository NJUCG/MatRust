#pragma once
#include"qwidget.h"
#include"EventAdapter.h"
#include"FloatEdit.h"
#include"ObjectData.h"

class ObjectTab: public QWidget, public FloatEditResponder, public EventResponder
{
public:
	ObjectTab();
	void set_name(string);
	void set_object(ObjectData);
	void set_property(string, float);
	ObjectData get_object();
	void on_value_changed(string, float) override;
	void on_trigger(string) override;
protected:
	QLabel* name_label;

	unordered_map<string, FloatEdit*> local_data;

	QVBoxLayout* top_layout;

	void init();
	void addComponent();

	void header();
	void add_property(string, string, string unit, float);
};

