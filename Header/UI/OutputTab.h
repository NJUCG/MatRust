#pragma once
#include"EventAdapter.h"
#include"FloatEdit.h"
#include"LightData.h"
#include"FileSelector.h"
#include"StringHelper.h"
#include"StringHelper.h"
#include<qimage.h>
#include<qlabel.h>
#include<unordered_map>
#include"ControllTabWidgetResponder.h"
#include"PipelineConfig.h"
#include"ImgDrawer.h"
#include"ExpandableNode.h"
#include<qfiledialog.h>
#include"ScalableContainer.h"
#include<qcheckbox.h>
using namespace std;

class OutputTab: public QWidget, public EventResponder, public ControllTabWidgetResponder
{
public:
	OutputTab();
	void on_trigger(string) override;
	void on_switch_to(int, int) override;
	void resizeEvent(QResizeEvent*) override;
protected:
	void init();
	void addComponent();
	void header(QString header);
	void body();
	void add_pic(QString title, string tag);
	void end_body();

	bool is_growing = false;
	PipelineConfig* config = nullptr;
	void show_growth();
	void hide_growth();
	
	QWidget* body_widget = nullptr;
	QVBoxLayout* body_layout = nullptr;
	QVBoxLayout* top_layout = nullptr;

	unordered_map<string, ImgDrawer*> imgs;

	QIcon repeat_icon;
	QIcon stretch_icon;
	QIcon save_icon;
};

