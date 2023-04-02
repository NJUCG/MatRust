#pragma once
#include<string>
using namespace std;
class NodeWidgetConfig {
public:

};
class NodeConnectionConfig {
public:
	int start_widget_idx;
	vec2 start_pos;

	int stop_widget_idx;
	vec2 stop_pos;

	string connection_value_type;
};
class NodeEditorConfig {
public:
	NodeEditorConfig() {

	}
	void load_config() {

	}
};