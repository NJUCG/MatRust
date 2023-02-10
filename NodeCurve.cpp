#include "NodeCurve.h"

NodeCurve::NodeCurve()
{
	start = stop = nullptr;
}

NodeCurve::NodeCurve(NodeDataButton* start, NodeDataButton* stop)
{
	this->start = start;
	this->stop = stop;
}

QPoint NodeCurve::get_start_point()
{
	return start->center();
}

QPoint NodeCurve::get_adapter_point()
{
	return (start->center() + stop->center()) / 2;
}

QPoint NodeCurve::get_stop_point()
{
	return stop->center();
}
