#include "EnvTab.h"

EnvTab::EnvTab()
{
	init();
	addComponent();
}

EnvTab::EnvTab(QWidget* parent): QWidget(parent)
{
	init();
	addComponent();
}

void EnvTab::on_trigger(string e)
{
	if (e == "combine_config_event_env_tab") {
		PipelineConfig* config = (PipelineConfig*)EventAdapter::shared->pop_data();
		config->textureWidth = config->textureHeight = 200;

		config->rh = arguments["rh"]->value;
		config->oc = arguments["oc"]->value;
		config->sc = arguments["sc"]->value;
		config->cc = arguments["cc"]->value;
		config->temperature = arguments["temperature"]->value;
		
		float m = arguments["metallic"]->value;
		float r = arguments["roughness"]->value;

		config->metallic = vector<vector<float>>(config->textureHeight, vector<float>(config->textureWidth, m));
		config->roughness = vector<vector<float>>(config->textureHeight, vector<float>(config->textureWidth, r));

		EventAdapter::shared->push_data(config);
	}
	else if (e == "new_mesh_loaded") {
		EventAdapter::shared->push_data(new float(arguments["metallic"]->value));
		EventAdapter::shared->trigger_event("model_back_metallic_changed");
		EventAdapter::shared->push_data(new float(arguments["roughness"]->value));
		EventAdapter::shared->trigger_event("model_back_roughness_changed");
	}
}

void EnvTab::on_value_changed(string name, float new_value)
{
	if (name == "metallic") {
		EventAdapter::shared->push_data(new float(new_value));
		EventAdapter::shared->trigger_event("model_back_metallic_changed");
	}
	else if (name == "roughness") {
		EventAdapter::shared->push_data(new float(new_value));
		EventAdapter::shared->trigger_event("model_back_roughness_changed");
	}
}

void EnvTab::set_env(EnvData data)
{
	*backup_config = data;
	EventAdapter::shared->push_data(&(backup_config->metallic));
	EventAdapter::shared->trigger_event("model_back_metallic_changed");
	EventAdapter::shared->push_data(&(backup_config->roughness));
	EventAdapter::shared->trigger_event("model_back_roughness_changed");
}

void EnvTab::init()
{
	arguments = unordered_map<string, FloatEdit*>();
	body_widgets = unordered_map<int, QWidget*>();
	EventAdapter::shared->register_event("combine_config_event_env_tab", this);
	EventAdapter::shared->register_event("new_mesh_loaded", this);

	setStyleSheet(CssLoader::load_css("env_tab.css"));

	backup_config = new EnvData();
	// 默认config
	backup_config->rh = 70.0f;
	backup_config->cc = 70.0f;
	backup_config->oc = 20.0f;
	backup_config->sc = 70.0f;

	backup_config->temperature = 300;

	backup_config->roughness = 0.2f;

	backup_config->metallic = 1.0f;
}

void EnvTab::addComponent()
{
	layout = new QVBoxLayout();
	
	layout->setContentsMargins(5, 5, 5, 5);
	// 设置表头
	header(ENV_TAB_ENV_ARGS_HEADER_TITLE);
	body();
	argument(ENV_TAB_ARG_RH, "rh", backup_config->rh, 0, 100, 0.1f);
	argument(ENV_TAB_ARG_CC, "cc", backup_config->cc, 0, 100, 0.1f);
	argument(ENV_TAB_ARG_SC, "sc", backup_config->sc, 0, 100, 0.1f);
	argument(ENV_TAB_ARG_OC, "oc", backup_config->oc, 0, 100, 0.1f);
	argument(ENV_TAB_ARG_TEMPERATURE, "temperature", backup_config->temperature, 273, 373, 0.1f);
	argument(ENV_TAB_ARG_ROUGHNESS, "roughness", backup_config->roughness, 0, 1, 0.01f, 0.01f);
	argument(ENV_TAB_ARG_METALLIC, "metallic", backup_config->metallic, 0, 1, 0.01f, 0.01f);
	end_body();

	layout->addWidget(new QWidget());

	setLayout(layout);

	set_env(*backup_config);
}

void EnvTab::header(QString header)
{
	QWidget* head = new QWidget();

	UIModel* model = UIModel::get();
	head->setFixedHeight(model->control_panel_header_height);

	head->setObjectName("head");

	QHBoxLayout* header_layout = new QHBoxLayout();
	header_layout->setContentsMargins(0,0,0,0);

	QPushButton* header_icon = new QPushButton();
	header_icon->setObjectName("header_icon");
	header_icon->setFixedWidth(model->control_panel_header_icon_width);
	header_icon->setCheckable(true);
	header_icon->setChecked(true);
	int cur_widget = widget_id;
	connect(header_icon, &QPushButton::clicked, [this, header_icon, cur_widget]()->void {
		if (!header_icon->isChecked()) {
			shrink_panel(cur_widget);
		}
		else {
			expand_panel(cur_widget);
		}
		});
	
	header_layout->addWidget(header_icon); 


	QLabel* value_label = new QLabel();
	value_label->setText(header);
	value_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	header_layout->addWidget(value_label);

	header_layout->addWidget(new QWidget());

	head->setLayout(header_layout);
	
	
	layout->addWidget(head);
}

void EnvTab::body()
{
	body_widget = new QWidget();
	body_layout = new QVBoxLayout();
	body_layout->setContentsMargins(0, 0, 0, 0);
	
	body_widget->setLayout(body_layout);
	layout->addWidget(body_widget);
}

void EnvTab::end_body()
{
	body_layout->addWidget(new QWidget());
	
	body_widgets[widget_id] = body_widget;
	body_layouts[widget_id] = body_layout;

	widget_id++;
}

void EnvTab::argument(QString name, string tag, float init_value, float min, float max, float step, float sensitivity)
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
	editor->sensitivity = sensitivity;
	arguments[tag] = editor;
	editor->setFixedWidth(UIModel::get()->control_panel_edit_width);
	editor->responder = this;
	editor->name = tag;
	line_layout->addWidget(editor);
	line->setLayout(line_layout);
	
	body_layout->addWidget(line);
}

void EnvTab::expand_panel(int widget_id)
{
	body_widgets[widget_id]->show();
}

void EnvTab::shrink_panel(int widget_id)
{
	body_widgets[widget_id]->hide();
}

void EnvTab::space()
{
	QWidget* space = new QWidget();
	int line_height = UIModel::get()->control_panel_line_height;
	space->setFixedHeight(line_height);
	body_layout->addWidget(space);
}


