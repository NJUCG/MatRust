#pragma once
#include<glm/glm.hpp>
using namespace glm;
class UIModel
{
public:
	static UIModel* get();
	//control tab的参数
	int control_tab_height;
	int control_tab_width;
	int control_selecotr_width;
	int control_selector_height;
	int control_selector_icon_width;
	int control_selector_icon_height;
	int control_selector_button_width;
	int control_selector_button_height;
	int float_edit_height;
	int float_edit_arrow_width;
	int control_panel_header_height;
	int control_panel_header_icon_width;
	int control_panel_line_height;
	int control_panel_single_component_width;
	int control_panel_edit_width;
	int node_widget_header_height;


	//entry的参数
	//子节点的偏移
	int node_left_offset;
	//其他内容相对右侧的偏移
	int node_right_offset;

	//
	int float_slider_height;
	int float_slider_title_label_width;
	int float_slider_value_label_width;
	int node_widget_width;
	//
	int node_button_width;
	int node_widget_line_height;

	//
	float current_time;

	int common_spacing;

	//
	vec3 init_loc;
	int src_width;
	int src_height;

	// 
	float canvas_view_move_sensitivity;
	float canvas_view_rotate_sensitivity;

	//
	int metallic_index;
	int roughness_index;
	int normal_disturb_index;
	int depth_index;
	int noise_index;

	//
	int scene_entry_unit_offset;
	int scene_entry_icon_width;
	int scene_entry_line_height;

	//
	int header_tab_width;
	
	int selector_height;
	int selector_width;

	//
	int header_height;
	//
	int control_panel_init_width;

private:
	static UIModel* model;
	UIModel();
};

