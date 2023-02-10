#pragma once
#include<qwidget.h>
#include"NodeDataButton.h"
class NodeCurve
{
public:
	NodeCurve();
	NodeCurve(NodeDataButton*,NodeDataButton*);
	NodeDataButton* start, *stop;
	QPoint get_start_point();
	QPoint get_adapter_point();
	QPoint get_stop_point();
};

