#include "ObjectTab.h"

ObjectTab::ObjectTab()
{
	init();
	addComponent();
}

void ObjectTab::set_name(string name)
{
	name_label->setText((QString::fromLocal8Bit(name.c_str())));
}

void ObjectTab::set_object(ObjectData* data)
{
	set_name(data->name);
	switch_tab_style(data->type, data);
	set_property("loc_x", data->loc.x);
	set_property("loc_y", data->loc.y);
	set_property("loc_z", data->loc.z);
	set_property("rot_x", data->rot.x);
	set_property("rot_y", data->rot.y);
	set_property("rot_z", data->rot.z);
	set_property("scl_x", data->scl.x);
	set_property("scl_y", data->scl.y);
	set_property("scl_z", data->scl.z);
	
	local_data["loc_x"]->update();
	local_data["loc_y"]->update();
	local_data["loc_z"]->update();
	local_data["rot_x"]->update();
	local_data["rot_y"]->update();
	local_data["rot_z"]->update();
	local_data["scl_x"]->update();
	local_data["scl_y"]->update();
	local_data["scl_z"]->update();

	current_obj = data;
}

void ObjectTab::set_property(string name, float data)
{
	if (local_data.find(name) != local_data.end()) {
		local_data[name]->set_value(data, false);
	}
}

float ObjectTab::get_property(string name)
{
	if (local_data.find(name) != local_data.end() && local_data[name] != nullptr) {
		FloatEdit* data = local_data[name];
		float t = data->value;
		return local_data[name]->value;
	}
	return 0;
}

void ObjectTab::switch_tab_style(string style, ObjectData* data)
{
	deactivate_property("lght_int");
	if (style == "light") {
		LightData* light_data = (LightData*)data;
		activate_property("lght_int");
		set_property("lght_int", light_data->light_intensity.x);
	}
}

ObjectData ObjectTab::get_object()
{
	if (!current_obj) {
		return ObjectData();
	}
	current_obj->loc.x = get_property("loc_x");
	current_obj->loc.y = get_property("loc_y");
	current_obj->loc.z = get_property("loc_z");

	current_obj->rot.x = get_property("rot_x");
	current_obj->rot.y = get_property("rot_y");
	current_obj->rot.z = get_property("rot_z");

	current_obj->scl.x = get_property("scl_x");
	current_obj->scl.y = get_property("scl_y");
	current_obj->scl.z = get_property("scl_z");

	if (current_obj->type == "light") {
		LightData* lg = (LightData*)current_obj;
		float li = get_property("lght_int");
		lg->light_intensity = vec3(li, li, li);
	}

	return *current_obj;
}

void ObjectTab::on_value_changed(string name, float data)
{
	get_object();
	EventAdapter::shared->push_data(current_obj);
	EventAdapter::shared->trigger_event("object_value_changed");
}

void ObjectTab::on_trigger(string name)
{
	if(name == "selected_object_changed") {
		ObjectData* obj = (ObjectData*)EventAdapter::shared->pop_data();
		set_object(obj);
	}
}

void ObjectTab::init()
{
	setStyleSheet(CssLoader::load_css("object_tab.css"));
	local_data = unordered_map<string, FloatEdit*>();
	label_data = unordered_map<string, QLabel*>();
	EventAdapter::shared->register_event("selected_object_changed", this);
}

void ObjectTab::addComponent()
{
	top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(5);
	header();
	UIModel* model = UIModel::get();
	int line_height = model->control_panel_line_height;
	QWidget* spacing = new QWidget();

	spacing->setFixedHeight(line_height);

	// 名字
	name_label = new QLabel();
	name_label->setStyleSheet("background-color:transparent;color:white;");
	name_label->setFixedHeight(line_height);
	top_layout->addWidget(name_label);

	add_property("Location x", "loc_x", "m", 0);
	add_property("         y", "loc_y", "m", 0);
	add_property("         z", "loc_z", "m", 0);

	top_layout->addWidget(spacing);

	add_property("  Rotate x", "rot_x", " ", 0);
	add_property("         y", "rot_y", " ", 0);
	add_property("         z", "rot_z", " ", 0);

	top_layout->addWidget(spacing);

	add_property("   Scale x", "scl_x", "%", 0);
	add_property("         y", "scl_y", "%", 0);
	add_property("         z", "scl_z", "%", 0);

	top_layout->addWidget(spacing);

	add_property("   Light Intensity", "lght_int", "", 0);

	deactivate_property("lght_int");

	top_layout->addWidget(spacing);

	top_layout->addWidget(new QWidget());

	setLayout(top_layout);

	ObjectData* default_object = new ObjectData();
	default_object->loc = vec4(0, 0, 0, 0);
	default_object->rot = vec4(0, 0, 0, 0);
	default_object->scl = vec4(1, 1, 1, 0);
	default_object->name = "      无";
	set_object(default_object);
	delete default_object;
}

void ObjectTab::header()
{
	QWidget* header = new QWidget();
	QHBoxLayout* header_layout = new QHBoxLayout();
	header_layout->setContentsMargins(0, 0, 0, 0);
	header_layout->setSpacing(0);
	header->setStyleSheet("background-color:transparent;color:white;");

	UIModel* model = UIModel::get();
	QPushButton* icon_btn = new QPushButton();
	icon_btn->setObjectName("header_icon");
	icon_btn->setStyleSheet("background-color:transparent;color:transparent;border:solid 0px;");

	QIcon* icon = new QIcon((string("resources/ui/icons/object-30.png").c_str()));
	icon_btn->setIcon(*icon);
	icon_btn->setIconSize(QSize(model->control_selector_icon_width, model->control_selector_icon_height));
	icon_btn->setFixedSize(QSize(model->control_selector_button_width, model->control_selector_button_height));
	icon_btn->setEnabled(false);
	
	header_layout->addWidget(icon_btn);

	QLabel* header_label = new QLabel();
	header_label->setText(QString::fromLocal8Bit(string("物体").c_str()));

	header_layout->addWidget(header_label);

	header->setLayout(header_layout);

	top_layout->addWidget(header);

	header->setFixedHeight(model->control_selector_button_width);
}

void ObjectTab::add_property(string prefix, string data_tag, string unit, float init_data)
{
	if (local_data.find(data_tag) != local_data.end()) {
		return;
	}
	
	UIModel* model = UIModel::get();
	int line_height = model->control_panel_line_height;
	// 一格数据
	FloatEdit* edit = new FloatEdit();
	edit->set_min_value(-100);
	edit->set_max_value(100);
	edit->unit = unit;
	edit->set_value(init_data);
	edit->responder = this;
	edit->name = data_tag;

	QWidget* x_w = new QWidget();
	x_w->setStyleSheet("background-color:transparent;color:white;");
	QHBoxLayout* x_l = new QHBoxLayout();
	QLabel* x_label = new QLabel();
	x_label->setText((QString::fromLocal8Bit(prefix.c_str())));
	x_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

	x_l->setContentsMargins(0, 0, 0, 0);
	x_l->setSpacing(5);
	x_w->setFixedHeight(line_height);

	x_l->addWidget(x_label);
	x_l->addWidget(edit);

	x_w->setLayout(x_l);

	top_layout->addWidget(x_w);

	local_data[data_tag] = edit;
	label_data[data_tag] = x_label;
}

void ObjectTab::deactivate_property(string name)
{
	if (local_data.find(name) != local_data.end()) {
		local_data[name]->setVisible(false);
		label_data[name]->setVisible(false);
	}
}

void ObjectTab::activate_property(string name)
{
	if (local_data.find(name) != local_data.end()) {
		local_data[name]->setVisible(true);
		label_data[name]->setVisible(true);
	}
}
