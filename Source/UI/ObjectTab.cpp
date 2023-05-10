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
	local_data.clear();
	clear_layout(top_layout);

	switch_tab_style(data->type, data);

	current_obj = data;
}

void ObjectTab::set_property(string name, float data)
{
	if (local_data.find(name) != local_data.end()) {
		local_data[name]->set_value(data, false);
		local_data[name]->update();
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

void ObjectTab::on_select_file(string tag, string data)
{
}

void ObjectTab::switch_tab_style(string style, ObjectData* data)
{
	header(data->type);
	if (style == "light") {
		LightData* light_data = (LightData*)data;

		UIModel* model = UIModel::get();
		int line_height = model->control_panel_line_height;
		QWidget* spacing = new QWidget();

		spacing->setFixedHeight(line_height);

		add_property(OBJECT_TAB_LOC_X, "loc_x", "m", 0);
		add_property(OBJECT_TAB_LOC_Y, "loc_y", "m", 0);
		add_property(OBJECT_TAB_LOC_Z, "loc_z", "m", 0);

		top_layout->addSpacing(line_height);

		add_property(OBJECT_TAB_ROT_X, "rot_x", " ", 0);
		add_property(OBJECT_TAB_ROT_Y, "rot_y", " ", 0);
		add_property(OBJECT_TAB_ROT_Z, "rot_z", " ", 0);

		top_layout->addSpacing(line_height);

		add_property(OBJECT_TAB_SCL_X, "scl_x", "%", 0);
		add_property(OBJECT_TAB_SCL_Y, "scl_y", "%", 0);
		add_property(OBJECT_TAB_SCL_Z, "scl_z", "%", 0);

		top_layout->addSpacing(line_height);

		add_property(OBJECT_TAB_LIGHT_INTENSITY, "lght_int", "", 0);

		set_property("lght_int", light_data->light_intensity.z);

		top_layout->addSpacing(line_height);

		top_layout->addWidget(new QWidget());
	}
	else if (style == "mesh") {
		UIModel* model = UIModel::get();
		int line_height = model->control_panel_line_height;
		QWidget* spacing = new QWidget();
		spacing->setFixedHeight(line_height);

		QWidget* selector_container = new QWidget();
		 
		QHBoxLayout* container_layout = new QHBoxLayout();
		container_layout->setContentsMargins(0, 0, 0, 0);
		container_layout->setSpacing(0);
		
		container_layout->addWidget(new QWidget());

		mesh_selector = new FileSelector();
		mesh_selector->set_name(OBJECT_TAB_OBJECT_MODEL_PATH);
		mesh_selector->event_name = "selected_mesh_changed";
		mesh_selector->tag = "mesh_path";
		if (data->has_path) {
			mesh_selector->set_normal(QString::fromStdString(data->path));
		}

		container_layout->addWidget(mesh_selector);

		selector_container->setLayout(container_layout);

		selector_container->setFixedHeight(line_height);

		top_layout->addWidget(selector_container);

		add_property(OBJECT_TAB_LOC_X, "loc_x", "m", 0);
		add_property(OBJECT_TAB_LOC_Y, "loc_y", "m", 0);
		add_property(OBJECT_TAB_LOC_Z, "loc_z", "m", 0);

		top_layout->addSpacing(line_height);

		add_property(OBJECT_TAB_ROT_X, "rot_x", " ", 0);
		add_property(OBJECT_TAB_ROT_Y, "rot_y", " ", 0);
		add_property(OBJECT_TAB_ROT_Z, "rot_z", " ", 0);

		top_layout->addSpacing(line_height);

		add_property(OBJECT_TAB_SCL_X, "scl_x", "%", 100,1,1000);
		add_property(OBJECT_TAB_SCL_Y, "scl_y", "%", 100,1,1000);
		add_property(OBJECT_TAB_SCL_Z, "scl_z", "%", 100,1,1000);
	}

	set_property("loc_x", data->loc.x);
	set_property("loc_y", data->loc.y);
	set_property("loc_z", data->loc.z);
	set_property("rot_x", data->rot.x);
	set_property("rot_y", data->rot.y);
	set_property("rot_z", data->rot.z);
	set_property("scl_x", data->scl.x);
	set_property("scl_y", data->scl.y);
	set_property("scl_z", data->scl.z);
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

	current_obj->scl.x = get_property("scl_x") / 100.0f;
	current_obj->scl.y = get_property("scl_y") / 100.0f;
	current_obj->scl.z = get_property("scl_z") / 100.0f;

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
	else if (name == "load_archive_event_obj") {
		QString* p = (QString*)EventAdapter::shared->pop_data();
		load_config(*p);
	}
	else if (name == "save_archive_event_obj") {
		QString* p = (QString*)EventAdapter::shared->pop_data();
		save_config(*p);
	}
}

void ObjectTab::init()
{
	setStyleSheet(CssLoader::load_css("object_tab.css"));
	local_data = unordered_map<string, FloatEdit*>();
	label_data = unordered_map<string, QLabel*>();
	EventAdapter::shared->register_event("selected_object_changed", this);
	EventAdapter::shared->register_event("load_archive_event_obj", this);
	EventAdapter::shared->register_event("save_archive_event_obj", this);
}

void ObjectTab::addComponent()
{
	top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(5);

	int t = top_layout->count();
	setLayout(top_layout);
	ObjectData* default_object = new ObjectData();
	default_object->loc = vec4(0, 0, 0, 0);
	default_object->rot = vec4(0, 0, 0, 0);
	default_object->scl = vec4(1, 1, 1, 0);
	default_object->name = "无";
	default_object->type = "mesh";
	set_object(default_object);

	delete default_object;
}

void ObjectTab::clear_layout(QLayout* layout)
{
	while (QLayoutItem* item = layout->takeAt(0))
	{
		if (QWidget* widget = item->widget()) {
			widget->deleteLater();
		}
		if (QLayout* child_layout = item->layout()) {
			clear_layout(child_layout);
		}
		if (QSpacerItem* spacer_item = item->spacerItem()) {
			layout->removeItem(spacer_item);
		}
		delete item;
	}
}

void ObjectTab::clear_properties()
{
}

void ObjectTab::header(string type)
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

	string icon_loc = "";
	if (type == "mesh") {
		icon_loc = "resources/ui/icons/object-30.png";
	}
	else if (type == "light") {
		icon_loc = "resources/ui/icons/light-30.png";
	}

	QIcon* icon = new QIcon((icon_loc.c_str()));
	
	icon_btn->setIcon(*icon);
	icon_btn->setIconSize(QSize(model->control_selector_icon_width, model->control_selector_icon_height));
	icon_btn->setFixedSize(QSize(model->control_selector_button_width, model->control_selector_button_height));
	icon_btn->setEnabled(false);
	
	header_layout->addWidget(icon_btn);

	QLabel* header_label = new QLabel();

	QString header_txt = "";
	if (type == "mesh") {
		header_txt = OBJECT_TAB_OBJECT_HEADER_TITLE;
	}
	else if (type == "light") {
		header_txt = OBJECT_TAB_LIGHT_HEADER_TITLE;
	}

	header_label->setText(header_txt);

	header_layout->addWidget(header_label);

	header->setLayout(header_layout);

	top_layout->addWidget(header);

	header->setFixedHeight(model->control_selector_button_width);
}

void ObjectTab::add_property(QString prefix, string data_tag, string unit, float init_data,float min,float max)
{
	if (local_data.find(data_tag) != local_data.end()) {
		return;
	}
	
	UIModel* model = UIModel::get();
	int line_height = model->control_panel_line_height;
	// 一格数据
	FloatEdit* edit = new FloatEdit();
	edit->set_min_value(min);
	edit->set_max_value(max);
	edit->set_unit(unit);
	edit->set_value(init_data);
	edit->responder = this;
	edit->name = data_tag;

	QWidget* x_w = new QWidget();
	x_w->setStyleSheet("background-color:transparent;color:white;");
	QHBoxLayout* x_l = new QHBoxLayout();
	x_l->setAlignment(Qt::AlignRight);

	QLabel* x_label = new QLabel();
	x_label->setText(prefix);
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

void ObjectTab::load_config(QString path)
{
	QFile load_file(path);

	if (!load_file.open(QIODeviceBase::ReadOnly)) {
		qWarning() << "config file read failed.";
		return;
	}

	QByteArray save_data = load_file.readAll();
	QJsonDocument load_doc(QJsonDocument::fromJson(save_data));

	QJsonObject json = load_doc.object();
	
	ObjectData* data = new ObjectData();
	data->type = "mesh";

	data->path = json["model_path"].toString().toStdString();

	data->loc.x = json["loc_x"].toDouble();
	data->loc.y = json["loc_y"].toDouble();
	data->loc.z = json["loc_z"].toDouble();

	data->rot.x = json["rot_x"].toDouble();
	data->rot.y = json["rot_y"].toDouble();
	data->rot.z = json["rot_z"].toDouble();

	data->scl.x = json["scl_x"].toDouble();
	data->scl.y = json["scl_y"].toDouble();
	data->scl.z = json["scl_z"].toDouble();

	set_object(data);
}

void ObjectTab::save_config(QString path)
{
	QJsonObject json;

	QFile save_file(path);

	if (!save_file.open(QIODeviceBase::WriteOnly)) {
		qWarning() << "node editor config save failed.";
		return;
	}

	save_file.write(QJsonDocument(json).toJson());
}
