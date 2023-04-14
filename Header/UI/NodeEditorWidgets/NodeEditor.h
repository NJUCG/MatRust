#pragma once
#include<qwidget.h>
#include"NodePool.h"
#include"CssLoader.h"
#include"qpainter.h"
#include<qevent.h>
#include"NodeWidget.h"
#include<unordered_map>
#include<vector>
#include"NodeCurve.h"
#include<qcursor.h>
#include<qpainterpath.h>
#include"NodeEditorPanel.h"
#include"CustomLayerNodeWidget.h"
#include"LayerMachineNodeWidget.h"
#include"EventAdapter.h"
#include"PipelineConfig.h"
#include"PerlinLayerNodeWidget.h"
#include"DPDLayerNodeWidget.h"
#include<qfiledialog.h>
using namespace std;

class NodeEditor: public QWidget, public NodeWidgetResponder, public NodeEditorPanelResponder, public EventResponder

{
public:
	NodeEditor();
	NodePool* get_pool() override;
	void release_on_me(QPoint) override;
	void reset_curving() override;
	void on_widget_move() override;
	void update_temp_curve(QPoint) override;
	void btn_down(string) override;
	void on_trigger(string) override;
	void start_linking(NodeDataButton*) override;
	void remove_connection(NodeDataButton*) override;

	void read_config(QString); 
	void write_config(QString);
protected: 
	NodePool* pool = nullptr;

	LayerMachineNodeWidget* machine = nullptr;

	NodeEditorPanel* panel = nullptr;
	vector<NodeWidget*> widgets;
	QVBoxLayout* top_layout = nullptr;

	void init();
	void addComponent();

	void btn_down(NodeWidget*);
	void open_panel();
	void close_panel();

	void add_dots();
	
	void paintEvent(QPaintEvent*) override;

	void mouseMoveEvent(QMouseEvent*) override;
	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	
	QPainterPath curve_path(QPoint, QPoint);

	void update_widgets(QPoint);

	// 移动了多少
	float offset_x, offset_y;
	bool mouse_down = false;
	bool mouse_middle_down = false;
	// 前一刻鼠标位置
	float delta_mouse_x, delta_mouse_y, prev_mouse_x = -1, prev_mouse_y = -1;
	// 曲线逻辑
	vector<NodeCurve*> curves;
	bool is_curving = false;
	NodeCurve* temp_curve = nullptr;
	QPoint current_pos;
};

