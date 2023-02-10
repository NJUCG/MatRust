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
using namespace std;
class NodeEditor: public QWidget, public NodeWidgetResponder
{
public:
	NodeEditor();
	NodePool* get_pool() override;
	void release_on_me(QPoint) override;
	void reset_curving() override;
	void on_widget_move() override;
	void update_temp_curve(QPoint) override;
protected:
	NodePool* pool = nullptr;
	vector<NodeWidget*> widgets;
	void init();
	void addComponent();

	void add_dots();
	
	void paintEvent(QPaintEvent*) override;

	void mouseMoveEvent(QMouseEvent*) override;
	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	
	QPainterPath curve_path(QPoint, QPoint);

	void update_widgets(QPoint);

	void start_linking(NodeDataButton*) override;
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

