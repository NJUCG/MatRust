#include "BakeTab.h"

BakeTab::BakeTab()
{
	init();
	addComponent();
}

void BakeTab::on_trigger(string name)
{
	if (name == "load_archive_event_bake_tab") {
		QString* p = (QString*)EventAdapter::shared->pop_data();
		load_config(*p);
	}
	else if (name == "save_archive_event_bake_tab") {
		QString* p = (QString*)EventAdapter::shared->pop_data();
		save_config(*p);
	}
}

void BakeTab::on_switch_to(int h, int w)
{

}

void BakeTab::on_value_changed(string name, float new_value)
{
	if (name == "stop_time") {
		EventAdapter::shared->push_data(new float(new_value));
		EventAdapter::shared->trigger_event("stop_time_changed");
		float* f = (float*)data_cache["stop_time"];
		*f = new_value;
	}
}

void BakeTab::init()
{
	setStyleSheet(CssLoader::load_css("bake_tab.css"));
	data_cache = unordered_map<string, void*>();
	widgets_cache = unordered_map<string, QWidget*>();

	data_cache["use_disturb"] = new bool(false);
	data_cache["use_depth"] = new bool(false);

	EventAdapter::shared->register_event("load_archive_event_bake_tab",this);
	EventAdapter::shared->register_event("save_archive_event_bake_tab", this);
}

void BakeTab::addComponent()
{
	top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(5, 5, 5, 5);

	header();

	illusion_widget = new QWidget();
	
	illusion_layout = new QVBoxLayout();
	illusion_layout->setContentsMargins(0, 0, 0, 0);
	illusion_layout->setSpacing(5);

	new_checkbox(BAKE_TAB_USE_DISTURB, "use_disturb");
	new_checkbox(BAKE_TAB_USE_DEPTH, "use_depth");
	
	illusion_widget->setLayout(illusion_layout);

	ExpandableNode* illusion_node = new ExpandableNode("Illusion", illusion_widget);
	
	top_layout->addWidget(illusion_node);
	
	settings_widget = new QWidget();
	settings_layout = new QVBoxLayout();
	settings_layout->setContentsMargins(0, 0, 0, 0);
	settings_layout->setSpacing(0);

	argument(BAKE_TAB_TIME, "stop_time", 20.0f, 5, 50, 0.1);

	settings_widget->setLayout(settings_layout);

	ExpandableNode* settings_node = new ExpandableNode("Illusion", settings_widget);

	top_layout->addWidget(settings_node);

	top_layout->addWidget(new QWidget());

	setLayout(top_layout);
}

void BakeTab::header()
{
	QWidget* header_widget = new QWidget();

	QHBoxLayout* header_layout = new QHBoxLayout();
	header_layout->setAlignment(Qt::AlignLeft);
	header_layout->setContentsMargins(5, 0, 5, 0);
	header_layout->setSpacing(5);
	
	header_widget->setStyleSheet("background-color:transparent;color:white;");

	UIModel* model = UIModel::get();
	QPushButton* icon_btn = new QPushButton();
	icon_btn->setObjectName("header_icon");
	icon_btn->setStyleSheet("background-color:transparent;color:transparent;border:solid 0px;");

	string icon_loc = "resources/ui/icons/bake.png";

	QIcon* icon = new QIcon((icon_loc.c_str()));

	icon_btn->setIcon(*icon);
	icon_btn->setIconSize(QSize(20, 20));
	icon_btn->setFixedSize(QSize(20, 20));
	icon_btn->setEnabled(false);

	header_layout->addWidget(icon_btn);

	QLabel* header_label = new QLabel();

	header_label->setText(BAKE_TAB_HEADER);

	header_layout->addWidget(header_label);

	header_widget->setLayout(header_layout);
	header_widget->setFixedHeight(model->control_selector_button_height);

	top_layout->addWidget(header_widget);
}

void BakeTab::new_checkbox(QString label, string tag)
{
	int line_height = UIModel::get()->control_panel_line_height;
	int comp_width = UIModel::get()->control_panel_single_component_width;

	QWidget* line_widget = new QWidget();
	line_widget->setFixedHeight(line_height);

	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);
	line_layout->setSpacing(0);

	QLabel* lab = new QLabel(label);
	lab->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

	QCheckBox* checkbox = new QCheckBox();
	checkbox->setTristate(false);
	widgets_cache[tag] = checkbox;

	connect(checkbox, &QCheckBox::stateChanged, this, [=](int state) {
		if (state == 0) {
			data_cache[tag] = new bool(false);
		}
		else if(state == 2){
			data_cache[tag] = new bool(true);
		}
		trigger_bake();
	});

	StretchContainer* c = new StretchContainer(lab, checkbox, 4, 6);
	line_layout->addWidget(c);

	line_widget->setLayout(line_layout);

	illusion_layout->addWidget(line_widget);
}

void BakeTab::trigger_bake()
{
	BakeInfo* info = new BakeInfo();
	info->use_disturb = *((bool*)data_cache["use_disturb"]);
	info->use_depth = *((bool*)data_cache["use_depth"]);

	EventAdapter::shared->push_data(info);
	EventAdapter::shared->trigger_event("bake_info_changed");
}

void BakeTab::save_config(QString path)
{
	QJsonObject json;

	json["use_disturb"] = *((bool*)data_cache["use_disturb"]);
	json["use_depth"] = *((bool*)data_cache["use_depth"]);
	json["stop_time"] = *((float*)(data_cache["stop_time"]));

	QFile save_file(path);
	//qDebug() << "save";
	if (!save_file.open(QIODeviceBase::WriteOnly)) {
		qWarning() << "node bake tag config save failed.";
		return;
	}

	save_file.write(QJsonDocument(json).toJson());
}

void BakeTab::load_config(QString path)
{
	QFile load_file(path);

	if (!load_file.open(QIODeviceBase::ReadOnly)) {
		qWarning() << "config file read failed.";
		return;
	}

	QByteArray save_data = load_file.readAll();
	QJsonDocument load_doc(QJsonDocument::fromJson(save_data));
	QJsonObject json = load_doc.object();

	QCheckBox* checkbox = (QCheckBox*)widgets_cache["use_disturb"];
	if (json["use_disturb"].toBool()) {
		checkbox->setCheckState(Qt::Checked);
	}else{
		checkbox->setCheckState(Qt::Unchecked);
	}
	data_cache["use_disturb"] = new bool(json["use_disturb"].toBool());

	checkbox = (QCheckBox*)widgets_cache["use_depth"];
	if (json["use_depth"].toBool()) {
		checkbox->setCheckState(Qt::Checked);
	}
	else {
		checkbox->setCheckState(Qt::Unchecked);
	}
	data_cache["use_depth"] = new bool(json["use_depth"].toBool());

	FloatEdit* editor = (FloatEdit*)widgets_cache["stop_time"];
	editor->set_value(json["stop_time"].toDouble());
	data_cache["stop_time"] = new float(json["stop_time"].toDouble());
	EventAdapter::shared->push_data(data_cache["stop_time"]);
	EventAdapter::shared->trigger_event("stop_time_changed");
}

void BakeTab::argument(QString name, string tag, float init_value, float min, float max, float step, float sensitivity)
{
	QWidget* line = new QWidget();
	int line_height = UIModel::get()->control_panel_line_height;
	line->setFixedHeight(line_height);

	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);
	//line_layout->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

	//line_layout->addWidget(new QWidget());

	QLabel* value_label = new QLabel();
	value_label->setText(name);
	value_label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	
	FloatEdit* editor = new FloatEdit();
	editor->set_min_value(min);
	editor->set_max_value(max);
	editor->set_value(init_value);
	editor->step = step;
	editor->set_unit("s");
	editor->sensitivity = sensitivity;
	data_cache[tag] = new float(editor->value);
	widgets_cache[tag] = editor;
	editor->responder = this;
	editor->name = tag;

	StretchContainer* container = new StretchContainer(value_label, editor, 4, 6);
	line_layout->addWidget(container);

	line->setLayout(line_layout);
	settings_layout->addWidget(line);
}