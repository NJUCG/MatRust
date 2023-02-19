#pragma once
#include"qwidget.h"
#include"EventAdapter.h"
#include"FloatEdit.h"
#include"LightData.h"

class ObjectTab: public QWidget, public FloatEditResponder, public EventResponder
{
public:
	ObjectTab();
	void set_name(string);
	void set_object(ObjectData*);
	void set_property(string, float);
	float get_property(string);

	void switch_tab_style(string, ObjectData* data);
	ObjectData get_object();
	void on_value_changed(string, float) override;
	void on_trigger(string) override;
protected:
	QLabel* name_label;
	ObjectData* current_obj = nullptr;

	unordered_map<string, FloatEdit*> local_data;
	unordered_map<string, QLabel*> label_data;

	QVBoxLayout* top_layout;

	void init();
	void addComponent();

	void header();
	void add_property(string, string, string unit, float);
	void deactivate_property(string);
	void activate_property(string);
};

