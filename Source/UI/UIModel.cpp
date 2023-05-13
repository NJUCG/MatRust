#include "UIModel.h"
UIModel* UIModel::model = nullptr;
UIModel::UIModel() {
	int factor = 2;
	control_tab_height = 600 * factor;
	control_tab_width = 200 * factor;
	control_selecotr_width = 30 * factor;
	control_selector_height = 600 * factor;
	control_selector_icon_height = control_selector_icon_width = 18 * factor;
	control_selector_button_height = control_selector_button_width = 25 * factor;
	node_left_offset = 10 * factor;
	float_edit_height = 20;
	float_edit_arrow_width = 15;
	control_panel_header_height = 20;
	control_panel_header_icon_width = 20 * factor;
	control_panel_line_height = control_panel_header_height;
	control_panel_edit_width = 80 * factor;

	node_widget_header_height = 20;

	float_slider_height = 20;
	float_slider_title_label_width = float_slider_value_label_width = 40;

	node_widget_width = 250;

	node_button_width = 10;

	current_time = 0;

	common_spacing = 0;

	init_loc = vec3(0, 0, 0);

	canvas_view_move_sensitivity = 0.02f;

	canvas_view_rotate_sensitivity = 0.1f;

	noise_index = 5;
	depth_index = 6;
	normal_disturb_index = 7;
	metallic_index = 8;
	roughness_index = 9;

	scene_entry_unit_offset = 20;

	scene_entry_icon_width = 20;

	scene_entry_line_height = 25;

	header_tab_width = 60;

	selector_height = 20;

	selector_width = 400;

	header_height = 30;

	node_widget_line_height = 20;

	control_panel_single_component_width = 120;
}
UIModel* UIModel::get()
{
	if (model == nullptr) {
		model = new UIModel();
	}
	return model;
}

