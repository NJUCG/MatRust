#pragma once
#include<qwidget.h>
#include"NodeDataButton.h"
#include<qjsonobject.h>
#include<qjsonarray.h>
#include<qjsondocument.h>
#include<glm/glm.hpp>
using namespace glm;
class NodeCurve
{
public:
	NodeCurve();
	NodeCurve(NodeDataButton*,NodeDataButton*);
	NodeDataButton* start, *stop;

	// 在最后保存时更新
	int start_widget_idx;
	int stop_widget_idx;

	// 在链接时更新
	QPoint start_pos;
	QPoint stop_pos;
	string connection_value_type;


	QPoint get_start_point();
	QPoint get_adapter_point();
	QPoint get_stop_point();
	void write(QJsonObject& json, QPoint base_loc) {
		json["start_widget_idx"] = start_widget_idx;
		json["stop_widget_idx"] = stop_widget_idx;

		json["start_pos_x"] = start_pos.x() - base_loc.x();
		json["start_pos_y"] = start_pos.y() - base_loc.y();

		json["stop_pos_x"] = stop_pos.x() - base_loc.x();
		json["stop_pos_y"] = stop_pos.y() - base_loc.y();
	}
};

