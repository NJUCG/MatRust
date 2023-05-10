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
	responders_dict = unordered_map<string, ControllTabWidgetResponder*>();
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

	BakeTab* bake_tab = new BakeTab();
	bake_tab->setObjectName("bake_tab");
	layout->addWidget(bake_tab);

	EnvTab* env_tab = new EnvTab();
	env_tab->setObjectName("env_tab");
	layout->addWidget(env_tab);
	
	ObjectTab* obj_tab = new ObjectTab();
	obj_tab->setObjectName("object_tab");
	layout->addWidget(obj_tab);

	//EngineTab* engine_tab = new EngineTab();
	//engine_tab->setObjectName("engine_tab");
	//layout->addWidget(engine_tab);

	LayerTab* layer_tab = new LayerTab();
	layer_tab->setObjectName("layer_tab");
	layout->addWidget(layer_tab);

	OutputTab* output_tab = new OutputTab();
	output_tab->setObjectName("output_tab");
	layout->addWidget(output_tab);

	tabs_dict["bake"] = bake_tab;
	responders_dict["bake"] = bake_tab;

	tabs_dict["env"] = env_tab;
	responders_dict["env"] = env_tab;

	tabs_dict["object-30"] = obj_tab;
	responders_dict["object-30"] = obj_tab;

	tabs_dict["layer-30"] = layer_tab;
	responders_dict["layer-30"] = layer_tab;

	tabs_dict["output-30"] = output_tab;
	responders_dict["output-30"] = output_tab;

	container->setLayout(layout);
	current_tab = obj_tab;
	current_responder = obj_tab;

	bake_tab->hide();
	env_tab->hide();
	layer_tab->hide();
	output_tab->hide();
	
	top_layout->addWidget(container);
	
	setLayout(top_layout);
}

void ControllPanel::switchTo(string type)
{
	current_tab->hide();
	current_responder->on_exit();

	current_tab = tabs_dict[type];
	current_responder = responders_dict[type];
	current_responder->on_switch_to(height(),width());
	
	current_tab->show();
}

void ControllPanel::value_changed(ControlTabParam arg, float newValue)
{
	
}


