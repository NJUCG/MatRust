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
	new_checkbox(BAKE_TAB_USE_DISTURB, "use_disturb");
	new_checkbox(BAKE_TAB_USE_DEPTH, "use_depth");
	argument(BAKE_TAB_TIME, "stop_time", 20.0f, 5, 50, 0.1);

	top_layout->addWidget(new QWidget());

	setLayout(top_layout);
}

void BakeTab::header()
{
	QWidget* header_widget = new QWidget();

	QHBoxLayout* header_layout = new QHBoxLayout();
	header_layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	header_layout->setContentsMargins(0, 0, 0, 0);
	header_layout->setSpacing(0);

	header_widget->setStyleSheet("background-color:transparent;color:white;");

	UIModel* model = UIModel::get();
	QPushButton* icon_btn = new QPushButton();
	icon_btn->setObjectName("header_icon");
	icon_btn->setStyleSheet("background-color:transparent;color:transparent;border:solid 0px;");

	string icon_loc = "resources/ui/icons/bake.png";

	QIcon* icon = new QIcon((icon_loc.c_str()));

	icon_btn->setIcon(*icon);
	icon_btn->setIconSize(QSize(model->control_selector_icon_width, model->control_selector_icon_height));
	icon_btn->setFixedSize(QSize(model->control_selector_button_width, model->control_selector_button_height));
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

	QWidget* line_widget = new QWidget();
	line_widget->setFixedHeight(line_height);

	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);
	line_layout->setSpacing(5);

	line_layout->addWidget(new QWidget());

	QLabel* lab = new QLabel(label);
	lab->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	line_layout->addWidget(lab);

	QCheckBox* checkbox = new QCheckBox();
	checkbox->setFixedWidth(20);
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
	line_layout->addWidget(checkbox);
	line_widget->setLayout(line_layout);

	top_layout->addWidget(line_widget);
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
	line_layout->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	line_layout->addWidget(new QWidget());
	QLabel* value_label = new QLabel();
	value_label->setText(name);
	value_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	line_layout->addWidget(value_label);

	FloatEdit* editor = new FloatEdit();
	editor->set_min_value(min);
	editor->set_max_value(max);
	editor->set_value(init_value);
	editor->step = step;
	editor->unit = "s";
	editor->sensitivity = sensitivity;
	data_cache[tag] = new float(editor->value);
	widgets_cache[tag] = editor;
	editor->setFixedWidth(UIModel::get()->control_panel_edit_width);
	editor->responder = this;
	editor->name = tag;
	line_layout->addWidget(editor);
	line->setLayout(line_layout);

	top_layout->addWidget(line);
}