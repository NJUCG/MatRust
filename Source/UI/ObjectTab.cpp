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
	switch_tab_style(data->type, data);
	current_obj = data;
}

void ObjectTab::set_property(string name, string tab, float data)
{
	name = tab + "/" + name;
	if (local_data.find(name) != local_data.end()) {
		local_data[name]->set_value(data, false);
		local_data[name]->update();
	}
}

float ObjectTab::get_property(string name)
{
	name = current_tab + "/" + name;
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

void ObjectTab::switch_tab_style(string tab, ObjectData* data)
{
	if (tab != current_tab) {
		current_tab = tab;

		if (tab == "mesh") {
			light_tab->hide();
			model_tab->show();
		}
		else if (tab == "light") {
			model_tab->hide();
			light_tab->show();
		}
	}

	if (tab == "light") {
		LightData* light_data = (LightData*)data;
		set_property("lght_int", tab, light_data->light_intensity.r);
	}

	set_property("loc_x", tab, data->loc.x);
	set_property("loc_y", tab, data->loc.y);
	set_property("loc_z", tab, data->loc.z);
	set_property("rot_x", tab, data->rot.x);
	set_property("rot_y", tab, data->rot.y);
	set_property("rot_z", tab, data->rot.z);
	set_property("scl_x", tab, data->scl.x);
	set_property("scl_y", tab, data->scl.y);
	set_property("scl_z", tab, data->scl.z);
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
	top_layout->setSpacing(0);

	add_light_tab();
	add_model_tab();

	setLayout(top_layout);

	light_tab->hide();

	ObjectData* default_object = new ObjectData();
	default_object->loc = vec4(0, 0, 0, 0);
	default_object->rot = vec4(0, 0, 0, 0);
	default_object->scl = vec4(100, 100, 100, 0);
	default_object->name = "无";
	default_object->type = "mesh";

	set_object(default_object);

	delete default_object;
}

void ObjectTab::add_light_tab()
{
	light_tab = new QWidget();
	light_tab->setStyleSheet("background-color: #3D3D3D;color:white;");

	light_layout = new QVBoxLayout();
	light_layout->setContentsMargins(5, 5, 5, 5);

	UIModel* model = UIModel::get();
	int line_height = model->control_panel_line_height;

	header(light_layout, "light");

	QWidget* loc_widget = new QWidget();

	QVBoxLayout* loc_layout = new QVBoxLayout();
	loc_layout->setContentsMargins(0, 0, 0, 0);
	loc_layout->setSpacing(5);

	add_property(loc_layout, OBJECT_TAB_LOC_X, "loc_x", "light", "m", 0);
	add_property(loc_layout, OBJECT_TAB_LOC_Y, "loc_y", "light", "m", 0);
	add_property(loc_layout, OBJECT_TAB_LOC_Z, "loc_z", "light", "m", 0);

	loc_widget->setLayout(loc_layout);

	ExpandableNode* loc_node = new ExpandableNode(OBJECT_TAB_LOC, loc_widget);

	light_layout->addWidget(loc_node);
	
	QWidget* rot_widget = new QWidget();

	QVBoxLayout* rot_layout = new QVBoxLayout();
	rot_layout->setContentsMargins(0, 0, 0, 0);
	rot_layout->setSpacing(5);

	add_property(rot_layout, OBJECT_TAB_ROT_X, "rot_x", "light", " ", 0);
	add_property(rot_layout, OBJECT_TAB_ROT_Y, "rot_y", "light", " ", 0);
	add_property(rot_layout, OBJECT_TAB_ROT_Z, "rot_z", "light", " ", 0);

	rot_widget->setLayout(rot_layout);

	ExpandableNode* rot_node = new ExpandableNode(OBJECT_TAB_ROT, rot_widget);

	light_layout->addWidget(rot_node);

	QWidget* scl_widget = new QWidget();

	QVBoxLayout* scl_layout = new QVBoxLayout();
	scl_layout->setContentsMargins(0, 0, 0, 0);
	scl_layout->setSpacing(5);

	add_property(scl_layout, OBJECT_TAB_SCL_X, "scl_x", "light", "%", 100, 1, 100);
	add_property(scl_layout, OBJECT_TAB_SCL_Y, "scl_y", "light", "%", 100, 1, 100);
	add_property(scl_layout, OBJECT_TAB_SCL_Z, "scl_z", "light", "%", 100, 1, 100);

	scl_widget->setLayout(scl_layout);

	ExpandableNode* scl_node = new ExpandableNode(OBJECT_TAB_SCL, scl_widget);

	light_layout->addWidget(scl_node);

	QWidget* int_widget = new QWidget();

	QVBoxLayout* int_layout = new QVBoxLayout();
	int_layout->setContentsMargins(0, 0, 0, 0);
	int_layout->setSpacing(5);

	add_property(int_layout, OBJECT_TAB_LIGHT_INTENSITY, "lght_int", "light", "", 0, 0, 100);

	int_widget->setLayout(int_layout);

	ExpandableNode* int_node = new ExpandableNode(OBJECT_TAB_LIGHT_INTENSITY, int_widget);

	light_layout->addWidget(int_node);

	light_layout->addWidget(new QWidget());

	light_tab->setLayout(light_layout);

	top_layout->addWidget(light_tab);
}

void ObjectTab::add_model_tab()
{
	model_tab = new QWidget();
	model_tab->setStyleSheet("background-color: #3D3D3D;color:white;");

	model_layout = new QVBoxLayout();
	model_layout->setContentsMargins(5, 5, 5, 5);

	UIModel* model = UIModel::get();
	int line_height = model->control_panel_line_height;

	header(model_layout, "mesh");

	mesh_selector = new FileSelector();
	mesh_selector->set_name(OBJECT_TAB_OBJECT_MODEL_PATH);
	mesh_selector->event_name = "selected_mesh_changed";
	mesh_selector->tag = "mesh_path";

	ExpandableNode* selector_node = new ExpandableNode(OBJECT_TAB_SELECT_MODEL, mesh_selector);
	
	model_layout->addWidget(selector_node);

	QWidget* loc_widget = new QWidget();

	QVBoxLayout* loc_layout = new QVBoxLayout();
	loc_layout->setContentsMargins(0, 0, 0, 0);
	loc_layout->setSpacing(5);

	add_property(loc_layout, OBJECT_TAB_LOC_X, "loc_x", "mesh", "m", 0);
	add_property(loc_layout, OBJECT_TAB_LOC_Y, "loc_y", "mesh", "m", 0);
	add_property(loc_layout, OBJECT_TAB_LOC_Z, "loc_z", "mesh", "m", 0);

	loc_widget->setLayout(loc_layout);

	ExpandableNode* loc_node = new ExpandableNode(OBJECT_TAB_LOC, loc_widget);

	model_layout->addWidget(loc_node);

	QWidget* rot_widget = new QWidget();

	QVBoxLayout* rot_layout = new QVBoxLayout();
	rot_layout->setContentsMargins(0, 0, 0, 0);
	rot_layout->setSpacing(5);

	add_property(rot_layout, OBJECT_TAB_ROT_X, "rot_x", "mesh", " ", 0);
	add_property(rot_layout, OBJECT_TAB_ROT_Y, "rot_y", "mesh", " ", 0);
	add_property(rot_layout, OBJECT_TAB_ROT_Z, "rot_z", "mesh", " ", 0);

	rot_widget->setLayout(rot_layout);

	ExpandableNode* rot_node = new ExpandableNode(OBJECT_TAB_ROT, rot_widget);

	model_layout->addWidget(rot_node);

	QWidget* scl_widget = new QWidget();

	QVBoxLayout* scl_layout = new QVBoxLayout();
	scl_layout->setContentsMargins(0, 0, 0, 0);
	scl_layout->setSpacing(5);

	add_property(scl_layout, OBJECT_TAB_SCL_X, "scl_x", "mesh", "%", 100, 1, 100);
	add_property(scl_layout, OBJECT_TAB_SCL_Y, "scl_y", "mesh", "%", 100, 1, 100);
	add_property(scl_layout, OBJECT_TAB_SCL_Z, "scl_z", "mesh", "%", 100, 1, 100);

	scl_widget->setLayout(scl_layout);

	ExpandableNode* scl_node = new ExpandableNode(OBJECT_TAB_SCL, scl_widget);

	model_layout->addWidget(scl_node);

	model_layout->addWidget(new QWidget());

	model_tab->setLayout(model_layout);

	top_layout->addWidget(model_tab);
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

void ObjectTab::header(QLayout* layout, string type)
{
	UIModel* model = UIModel::get();

	QWidget* header = new QWidget();
	header->setFixedHeight(model->control_selector_button_height);

	QHBoxLayout* header_layout = new QHBoxLayout();
	header_layout->setContentsMargins(5, 0, 5, 0);
	header_layout->setSpacing(5);
	header->setStyleSheet("background-color:transparent;color:white;border:0px solid red;");

	QPushButton* icon_btn = new QPushButton();
	icon_btn->setObjectName("header_icon");
	icon_btn->setStyleSheet("background-color:transparent;color:transparent;border:solid 0px;");

	QString icon_loc = "";
	if (type == "mesh") {
		icon_loc = "resources/ui/icons/object-30.png";
	}
	else if (type == "light") {
		icon_loc = "resources/ui/icons/light-30.png";
	}

	QIcon* icon = new QIcon((icon_loc));
	
	icon_btn->setIcon(*icon);
	icon_btn->setIconSize(QSize(20, 20));
	icon_btn->setFixedSize(QSize(20, 20));
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

	layout->addWidget(header);
}

void ObjectTab::add_property(QLayout* layout, QString prefix, string data_tag, string tab_tab, string unit, float init_data,float min,float max)
{
	if (local_data.find(tab_tab + "/" + data_tag) != local_data.end()) {
		return;
	}
	
	data_tag = tab_tab + "/" + data_tag;
	
	UIModel* model = UIModel::get();
	int line_height = model->control_panel_line_height;

	QWidget* line_widget = new QWidget();
	line_widget->setStyleSheet("background-color:transparent;color:white;");
	line_widget->setFixedHeight(line_height);

	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);
	line_layout->setSpacing(5);

	QLabel* value_label = new QLabel();
	value_label->setText(prefix);
	value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

	line_layout->addWidget(value_label);

	// 一格数据
	FloatEdit* edit = new FloatEdit();
	edit->set_min_value(min);
	edit->set_max_value(max);
	edit->set_unit(unit);
	edit->set_value(init_data);
	edit->responder = this;
	edit->name = data_tag;

	line_layout->addWidget(edit);

	line_widget->setLayout(line_layout);

	layout->addWidget(line_widget);

	local_data[data_tag] = edit;
	label_data[data_tag] = value_label;
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
