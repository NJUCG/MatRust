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
#include"NodeLineEditor.h"
#include"NodeWidgetPalette.h"
#include"LayerConfig.h"
#include"NodeWidgetLayerConfigLabel.h"
#include"StringHelper.h"
#include<qjsonobject.h>
#include<qjsonarray.h>
#include<qjsondocument.h>
#include"NodeEditorData.h"
using namespace std;

class NodeWidget:public QWidget, public NodeDataButtonResponder , public LabeledComboBoxResponder
{
public:
	NodeWidget(NodeWidgetResponder*,int,int, bool father = true);
	NodeWidget(NodeWidgetResponder*, QJsonObject, int loc_x, int loc_y);
	float loc_x, loc_y;
	QString type;
	void value_changed(QString, QString) override;
	NodeDataButton* release_in_me(QPoint);
	void update_temp_curve(QPoint) override;
	int get_node_data(string tag);
	virtual void read(const QJsonObject&);
	virtual void write(QJsonObject&);

	void destroy_widget();
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
	QString title;
	string title_color;
	//供子类重写用
	virtual void prepare_arguments();
	void header();
	virtual void body();
	
	int new_float_argument(QString title, string tag, bool has_source, bool has_adapter, float min = 0.0f, float max = 100.0f, float delta = 100.0f);
	int new_line(QString title, string tag, vector<QString> item_list, bool has_source = false, bool has_adapter = false);
	int new_label(QString init_text, string tag, bool has_source = false, bool has_adapter = false, bool tagged = false,QString label_tag = "", int tag_width = 40);
	int new_line_edit(QString init_text, string tag, bool has_source = false, bool has_adapter = false, bool tagged = false, QString label_tag = "", int tag_width = 40);
	int new_palette(string tag, bool interactable = true, bool has_source = false, bool has_adapter = false, bool tagged = false, QString label_tag = "", int tag_width = 40);
	
	// 这个用于传输已经存在的layer config , 方便widget对config进行更多操作
	int new_layer_config(LayerConfig* c, string tag, int name_data_t, bool has_source = true, bool tagged = false, QString label_tag = "", int tag_width = 40);
	// 这个用于接受或传输Layer config
	int new_layer_config(LayerConfig* c, string tag, bool has_source = false, bool has_adapter = false, bool tagged = false, QString label_tag = "", int tag_width = 40);

	// t为id
	void add_linker(QWidget* line, int t, bool has_source, bool has_adapter, int height);

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
	void remove_connection(int id) override;

	// 维护loc_x,loc_y用
	QPoint prev_position;
};

