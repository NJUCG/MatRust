#include "BakeTab.h"

BakeTab::BakeTab()
{
	init();
	addComponent();
	reload_noise();
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
	else if (name == "pipeline_started") {
		config = (PipelineConfig*)EventAdapter::shared->top_data();
	}
	else if (name == "noise_inited") {
		vector<vector<vec4>>* data = (vector<vector<vec4>>*)(EventAdapter::shared->pop_data());
		pic_widget->reset_img((*data).size(), (*data)[0].size(), data);
		pic_widget->update();
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
	else if (name == "noise_wavelength") {
		float* f = (float*)data_cache["noise_wavelength"];
		*f = new_value;
	}
	else if (name == "disturb_factor") {
		float* f = (float*)data_cache["disturb_factor"];
		*f = new_value;
		trigger_bake();
	}
}

void BakeTab::init()
{
	setStyleSheet(CssLoader::load_css("bake_tab.css"));
	data_cache = unordered_map<string, void*>();
	widgets_cache = unordered_map<string, QWidget*>();

	data_cache["use_disturb"] = new bool(false);
	data_cache["use_depth"] = new bool(false);

	repeat_icon = QIcon("resources/ui/icons/repeat-30.png");
	stretch_icon = QIcon("resources/ui/icons/stretch-30.png");

	EventAdapter::shared->register_event("load_archive_event_bake_tab",this);
	EventAdapter::shared->register_event("save_archive_event_bake_tab", this);
	EventAdapter::shared->register_event("noise_inited", this);
	EventAdapter::shared->register_event("pipeline_started", this);
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
	argument(illusion_layout, BAKE_TAB_DISTURB_FACTOR, "disturb_factor", "", 20.0f, 1.0f, 100.0f, 0.1f);
	new_checkbox(BAKE_TAB_USE_DEPTH, "use_depth");
	
	illusion_widget->setLayout(illusion_layout);

	ExpandableNode* illusion_node = new ExpandableNode(BAKE_TAB_ILLUSION, illusion_widget);
	
	top_layout->addWidget(illusion_node);
	
	settings_widget = new QWidget();
	settings_layout = new QVBoxLayout();
	settings_layout->setContentsMargins(0, 0, 0, 0);
	settings_layout->setSpacing(0);

	argument(settings_layout, BAKE_TAB_TIME, "stop_time", "s", 20.0f, 5, 50, 0.1);

	settings_widget->setLayout(settings_layout);

	ExpandableNode* settings_node = new ExpandableNode(BAKE_TAB_TIME, settings_widget);

	top_layout->addWidget(settings_node);

	noise_widget = new QWidget();
	noise_layout = new QVBoxLayout();
	noise_layout->setContentsMargins(0, 0, 0, 0);
	noise_layout->setSpacing(5);

	argument(noise_layout, BAKE_TAB_WAVE_LENGTH, "noise_wavelength", "", 10.0f, 1, 100, 1.0f);

	QPushButton* noise_btn = new QPushButton();
	noise_btn->setStyleSheet("QPushButton{background-color: #545454;}QPushButton:hover{background-color: #656565;}");
	noise_btn->setText(BAKE_TAB_REGENERATE_NOISE);
	noise_btn->setIcon(QIcon("resources/ui/icons/noise-30.png"));
	noise_btn->setFixedHeight(20);
	noise_btn->setMinimumHeight(20);
	connect(noise_btn, &QPushButton::clicked, [=]() {
		float* f = (float*)data_cache["noise_wavelength"];
		EventAdapter::shared->push_data(f);
		EventAdapter::shared->trigger_event("regenerate_noise_map");
		});
	
	noise_layout->addWidget(noise_btn);

	pic_widget = new ImgDrawer();

	QPushButton* view_mode_btn = new QPushButton();
	view_mode_btn->setObjectName("operator");
	view_mode_btn->setStyleSheet("QPushButton{background-color: #545454;}QPushButton:hover{background-color: #656565;}");
	view_mode_btn->setText(IMG_DRAWER_SWITCH_VIEW_MODE);
	view_mode_btn->setIcon(repeat_icon);
	view_mode_btn->setFixedHeight(20);
	view_mode_btn->setMinimumHeight(20);
	connect(view_mode_btn, &QPushButton::clicked, [=]() {
		ImgDrawer::Policy policy = pic_widget->switchPolicy();
		if (policy == ImgDrawer::Policy::Single) {
			view_mode_btn->setIcon(stretch_icon);
		}
		else if (policy == ImgDrawer::Policy::Repeat) {
			view_mode_btn->setIcon(repeat_icon);
		}
		pic_widget->update();
		});
	
	noise_layout->addWidget(view_mode_btn);

	ScalableContainer* pic_c = new ScalableContainer(pic_widget);
	pic_c->setFixedHeight(120);

	ExpandableNode* preview_node = new ExpandableNode(IMG_DRAWER_PREVIEW, pic_c);

	noise_layout->addWidget(preview_node);

	noise_widget->setLayout(noise_layout);

	ExpandableNode* noise_node = new ExpandableNode(BAKE_TAB_NOISE, noise_widget);
	
	top_layout->addWidget(noise_node);

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
	info->disturb_factor = *(float*)data_cache["disturb_factor"];

	EventAdapter::shared->push_data(info);
	EventAdapter::shared->trigger_event("bake_info_changed");
}

void BakeTab::save_config(QString path)
{
	QJsonObject json;

	json["use_disturb"] = *((bool*)data_cache["use_disturb"]);
	json["use_depth"] = *((bool*)data_cache["use_depth"]);
	json["stop_time"] = *((float*)(data_cache["stop_time"]));
	json["disturb_factor"] = *((float*)(data_cache["disturb_factor"]));

	QFile save_file(path);
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

	editor = (FloatEdit*)(widgets_cache["disturb_factor"]);
	editor->set_value(json["disturb_factor"].toDouble());
	data_cache["disturb_factor"] = new float(json["disturb_factor"].toDouble());
	trigger_bake();
}

void BakeTab::reload_noise()
{
	EventAdapter::shared->trigger_event("load_noise_map");
}

void BakeTab::argument(QLayout* layout, QString name, string tag, string unit,  float init_value, float min, float max, float step, float sensitivity)
{
	QWidget* line = new QWidget();
	int line_height = UIModel::get()->control_panel_line_height;
	line->setFixedHeight(line_height);

	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);

	QLabel* value_label = new QLabel();
	value_label->setText(name);
	value_label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	
	FloatEdit* editor = new FloatEdit();
	editor->set_min_value(min);
	editor->set_max_value(max);
	editor->set_value(init_value);
	editor->step = step;
	editor->set_unit(unit);
	editor->sensitivity = sensitivity;
	data_cache[tag] = new float(editor->value);
	widgets_cache[tag] = editor;
	editor->responder = this;
	editor->name = tag;

	StretchContainer* container = new StretchContainer(value_label, editor, 4, 6);
	line_layout->addWidget(container);

	line->setLayout(line_layout);
	
	layout->addWidget(line);
}