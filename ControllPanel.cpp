#include "ControllPanel.h"

ControllPanel::ControllPanel()
{
	init();
	addComponent();
}

void ControllPanel::init()
{
	model = UIModel::get();
	tabs_dict = unordered_map<string, QWidget*>();
	current_tab = 0;
	setMinimumWidth(model->control_tab_width);
	setStyleSheet(CssLoader::load_css("controller_panel_style.css"));
}

void ControllPanel::addComponent()
{
	QHBoxLayout* top_layout = new QHBoxLayout();
	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(0);
	
	ControllerSelector* selector = new ControllerSelector(this);
	selector->setObjectName("selector");
	selector->setFixedWidth(model->control_selecotr_width);
	
	top_layout->addWidget(selector);
	
	container = new QWidget();
	layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	
	QWidget* default_tab = new QWidget();

	default_tab->setObjectName("options");
	QLabel* value_label = new QLabel(default_tab);
	value_label->setText("hello tab.");
	layout->addWidget(default_tab);

	QWidget* bake_tab = new QWidget();
	bake_tab->setObjectName("options");
	QLabel* bake_label = new QLabel(bake_tab);
	bake_label->setText("hello bake.");
	layout->addWidget(bake_tab);

	EnvTab* env_tab = new EnvTab();
	env_tab->setObjectName("env_tab");
	layout->addWidget(env_tab);
	
	ObjectTab* obj_tab = new ObjectTab();
	obj_tab->setObjectName("object_tab");
	layout->addWidget(obj_tab);

	EngineTab* engine_tab = new EngineTab();
	engine_tab->setObjectName("engine_tab");
	layout->addWidget(engine_tab);

	tabs_dict["default"] = default_tab;
	tabs_dict["bake"] = bake_tab;
	tabs_dict["env"] = env_tab;
	tabs_dict["object-30"] = obj_tab;
	tabs_dict["engine-30"] = engine_tab;

	container->setLayout(layout);
	current_tab = default_tab;

	bake_tab->hide();
	env_tab->hide();
	obj_tab->hide();
	engine_tab->hide();
	
	top_layout->addWidget(container);
	
	setLayout(top_layout);
}

void ControllPanel::switchTo(string type)
{
	current_tab->hide();
	current_tab = tabs_dict[type];
	current_tab->show();
}

void ControllPanel::value_changed(ControlTabParam arg, float newValue)
{
}


