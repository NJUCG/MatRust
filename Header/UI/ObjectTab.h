#pragma once
#include"EventAdapter.h"
#include"FloatEdit.h"
#include"LightData.h"
#include"FileSelector.h"
#include"StringHelper.h"
#include"StringHelper.h"
#include"ControllTabWidgetResponder.h"
#include"ExpandableNode.h"
#include"StretchContainer.h"

class ObjectTab: public QWidget, public FloatEditResponder, public EventResponder, public FileSelectorResponder, public ControllTabWidgetResponder
{
public:
	ObjectTab();
	void set_name(string);
	void set_object(ObjectData*);
	void set_property(string, string, float);
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

	void add_light_tab();
	void add_model_tab();

	void clear_layout(QLayout*);
	void clear_properties();

	void header(QLayout* layout, string type);
	void add_property(QLayout*, QString, string, string, string unit, float,float min = -100,float max = 100);
	void deactivate_property(string);
	void activate_property(string);

	// ≥÷æ√ªØ
	void load_config(QString);
	void save_config(QString);

	// ÷ÿ÷√object tab
	QWidget* light_tab = nullptr;
	QVBoxLayout* light_layout = nullptr;

	QWidget* model_tab = nullptr;
	QVBoxLayout* model_layout = nullptr;

	string current_tab = "mesh";
};

