#pragma once
#include<qwidget.h>
#include"NodeWidgetResponder.h"
#include"CssLoader.h"
#include"qlayout.h"
#include<qlabel.h>
#include<qtextcodec.h>
#include"UIModel.h"
#include"FloatScrollBar.h"
#include"FloatProgressBarResponder.h"
#include<unordered_map>
#include"LabeledComboBox.h"
#include"NodeWidgetLabel.h"
using namespace std;

class NodeWidget:public QWidget, public NodeDataButtonResponder , public LabeledComboBoxResponder
{
public:
	NodeWidget(NodeWidgetResponder*,int,int, bool father = true);
	float loc_x, loc_y;
	void value_changed(string, string) override;
	NodeDataButton* release_in_me(QPoint);
	void update_temp_curve(QPoint) override;
protected:
	QWidget* wrapper_widget;
	NodePool* pool;
	NodeWidgetResponder* responder;
	QVBoxLayout* wrapper_layout;
	QVBoxLayout* top_layout;
	QVBoxLayout* body_layout;
	QWidget* header_widget;
	QWidget* body_widget;
	UIModel* model;
	IdGenerator* id_generator;

	unordered_map<string, int> data_cache;
	vector<NodeDataButton*> buttons;
	void init();
	void addComponent();

	//一些参数
	string title;
	string title_color;
	//供子类重写用
	virtual void prepare_arguments();
	void header();
	virtual void body();
	
	int add_float_argument(string title, bool has_source, bool has_adapter);
	int new_line(string title, vector<string> item_list, bool has_source = false, bool has_adapter = false);
	int new_label(string init_text, bool has_source = false, bool has_adapter = false);

	bool dragging = false;
	QPoint start_position;
	QPoint frame_position;
	// 布局用
	int current_height = 0;

	void mousePressEvent(QMouseEvent*) override;
	void mouseMoveEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void start_linking(int id) override;
	void release_on_me(QPoint) override;
};

