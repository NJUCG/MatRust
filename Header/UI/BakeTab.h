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
#include"ControllTabWidgetResponder.h"
#include"ExpandableNode.h"
#include"StretchContainer.h"
#include"ImgDrawer.h"
#include"ScalableContainer.h"
#include"PipelineConfig.h"

class BakeTab: public QWidget, public EventResponder, public FloatEditResponder, public ControllTabWidgetResponder
{
public:
	BakeTab();
	void on_trigger(string) override;
	void on_switch_to(int, int) override;
	void on_value_changed(string name, float new_value) override;
protected:
	QVBoxLayout* top_layout = nullptr;
	
	QWidget* illusion_widget = nullptr;
	QVBoxLayout* illusion_layout = nullptr;

	QWidget* settings_widget = nullptr;
	QVBoxLayout* settings_layout = nullptr;

	PipelineConfig* config = nullptr;
	QWidget* noise_widget = nullptr;
	QVBoxLayout* noise_layout = nullptr;

	ImgDrawer* pic_widget = nullptr;

	unordered_map<string, void*> data_cache;
	unordered_map<string, QWidget*> widgets_cache;

	void init();
	void addComponent();
	void argument(QLayout* layout, QString name, string tag, string unit, float init_value = 0, float min = 0, float max = 100, float step = 0.1, float sensitivity = 1);
	void header();
	void new_checkbox(QString label, string tag);

	void trigger_bake();

	void save_config(QString);
	void load_config(QString);

	void reload_noise();

	QIcon repeat_icon;
	QIcon stretch_icon;
};

