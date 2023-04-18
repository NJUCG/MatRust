#pragma once
#include"EventAdapter.h"
#include<qwidget.h>
#include"CssLoader.h"
#include<qcheckbox.h>
#include<qlayout.h>
#include<unordered_map>
#include<qlabel.h>
#include"UIModel.h"
#include<qpushbutton.h>
#include"BakeInfo.h"
#include"StringHelper.h"
#include"FloatEdit.h"

class BakeTab:public EventResponder, public QWidget, FloatEditResponder
{
public:
	BakeTab();
	void on_trigger(string) override;
	void on_value_changed(string name, float new_value) override;
protected:
	QVBoxLayout* top_layout = nullptr;

	unordered_map<string, void*> data_cache;
	unordered_map<string, QWidget*> widgets_cache;

	void init();
	void addComponent();
	void argument(QString name, string tag, float init_value = 0, float min = 0, float max = 100, float step = 0.1, float sensitivity = 1);
	void header();
	void new_checkbox(QString label, string tag);

	void trigger_bake();

	void save_config(QString);
	void load_config(QString);
};

