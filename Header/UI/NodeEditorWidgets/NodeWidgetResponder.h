#pragma once
#include"NodePool.h"
#include"NodeDataButton.h"
#include<qwidget.h>
class NodeWidgetResponder
{
public:
	virtual NodePool* get_pool();
	virtual void start_linking(NodeDataButton*);
	virtual void release_on_me(QPoint);
	virtual void reset_curving();
	virtual void on_widget_move();
	virtual void update_temp_curve(QPoint);
	virtual void remove_connection(NodeDataButton*);
	virtual void on_widget_deleted(void*);
};

