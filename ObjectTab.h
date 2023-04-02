#pragma once
#include"qwidget.h"
#include"EventAdapter.h"
#include"FloatEdit.h"
#include"LightData.h"
#include"FileSelector.h"
#include"StringHelper.h"
#include"StringHelper.h"

class ObjectTab: public QWidget, public FloatEditResponder, public EventResponder, public FileSelectorResponder
{
public:
	ObjectTab();
	void set_name(string);
	void set_object(ObjectData*);
	void set_property(string, float);
	float get_property(string);
	void on_select_file(string, string) override;

	void switch_tab_style(string, ObjectData* data);
	ObjectData get_object();
	void on_value_changed(string, float) override;
	void on_trigger(string) override;
protected:
	FileSelector* mesh_selector;

	QLabel* name_label;
	ObjectData* current_obj = nullptr;

	unordered_map<string, FloatEdit*> local_data;
	unordered_map<string, QLabel*> label_data;

	int first = 0;

	QVBoxLayout* top_layout = nullptr;

	void init();
	void addComponent();
	void clear_layout(QLayout*);
	void clear_properties();

	void header(string type);
	void add_property(QString, string, string unit, float,float min = -100,float max = 100);
	void deactivate_property(string);
	void activate_property(string);
};

