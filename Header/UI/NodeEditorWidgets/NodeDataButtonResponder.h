#pragma once
#include<qwidget.h>
class NodeDataButtonResponder
{
public:
	virtual void start_linking(int id);
	virtual void release_on_me(QPoint);
	virtual void update_temp_curve(QPoint);
	virtual void remove_connection(int id);
};

