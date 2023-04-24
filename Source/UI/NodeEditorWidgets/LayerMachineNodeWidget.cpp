#include "LayerMachineNodeWidget.h"

LayerMachineNodeWidget::LayerMachineNodeWidget(NodeWidgetResponder* responder, int loc_x, int loc_y) : NodeWidget(responder, loc_x, loc_y, false)
{
	init();
	addComponent();
}

void LayerMachineNodeWidget::value_changed(void*, int)
{
}

vector<LayerConfig*> LayerMachineNodeWidget::get_layers()
{
	vector<LayerConfig*> ret;
	
	for (int i = 0; i < 5; i++) {
		string idx = std::to_string(i);
		if (pool->get_value<LayerConfig*>(data_cache[idx])) {
			ret.push_back(pool->get_value<LayerConfig*>(data_cache[idx]));
		}
	}

	return ret;
}

void LayerMachineNodeWidget::read(const QJsonObject& json)
{
	NodeWidget::read(json);
}

void LayerMachineNodeWidget::write(QJsonObject& json)
{
	json["type"] = "layer_machine";
	NodeWidget::write(json);
}

void LayerMachineNodeWidget::init()
{
	NodeWidget::init();
	prepare_arguments();
}

void LayerMachineNodeWidget::addComponent()
{
	wrapper_layout = new QVBoxLayout();
	wrapper_layout->setContentsMargins(10, 10, 10, 10);

	wrapper_widget = new QWidget();
	top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(0);
	header();
	body_widget = new QWidget();
	body_widget->setObjectName("body_widget");
	body_layout = new QVBoxLayout();
	body_layout->setSpacing(5);
	body_layout->setContentsMargins(5, 5, 5, 5);
	current_height = 15 + model->node_widget_header_height;
	LayerMachineNodeWidget::body();
	body_widget->setLayout(body_layout);
	top_layout->addWidget(body_widget);
	top_layout->addWidget(new QWidget());
	wrapper_widget->setLayout(top_layout);

	wrapper_layout->addWidget(wrapper_widget);

	setObjectName("outer");
	setLayout(wrapper_layout);
}

void LayerMachineNodeWidget::prepare_arguments()
{
	title = NODE_EDITOR_NODE_WIDGET_LAYER_MACHINE_TITLE;
	title_color = string("background-color:#2B652B;");
}

void LayerMachineNodeWidget::body()
{
	new_layer_config(nullptr, "0", false, true, true, NODE_EDITOR_NODE_WIDGET_ENTRY_LAYER_0);
	new_layer_config(nullptr, "1", false, true, true, NODE_EDITOR_NODE_WIDGET_ENTRY_LAYER_1);
	new_layer_config(nullptr, "2", false, true, true, NODE_EDITOR_NODE_WIDGET_ENTRY_LAYER_2);
	new_layer_config(nullptr, "3", false, true, true, NODE_EDITOR_NODE_WIDGET_ENTRY_LAYER_3);
	new_layer_config(nullptr, "4", false, true, true, NODE_EDITOR_NODE_WIDGET_ENTRY_LAYER_4);
}

void LayerMachineNodeWidget::add_layer_slot()
{
}

void LayerMachineNodeWidget::remove_layer_slot()
{
}

void LayerMachineNodeWidget::add_remove_btn()
{
	int line_height = model->node_widget_line_height;

	QWidget* container = new QWidget();
	container->setFixedHeight(line_height);

	QHBoxLayout* add_remove_layout = new QHBoxLayout();
	add_remove_layout->setContentsMargins(5, 0, 5, 0);
	add_remove_layout->setSpacing(20);
	add_remove_layout->setAlignment(Qt::AlignCenter);
	
	QPushButton* left_btn = new QPushButton();
	QIcon* icon_left = new QIcon((string("resources/ui/icons/") + "plus-30" + string(".png")).c_str());
	left_btn->setIcon(*icon_left);
	left_btn->setIconSize(QSize(line_height, line_height));
	left_btn->setFixedSize(QSize(line_height, line_height));
	connect(left_btn, &QPushButton::clicked, [=]() {

		});
	add_remove_layout->addWidget(left_btn);

	QPushButton* right_btn = new QPushButton();
	QIcon* icon_right = new QIcon((string("resources/ui/icons/") + "subtract-30" + string(".png")).c_str());
	right_btn->setIcon(*icon_right);
	right_btn->setIconSize(QSize(line_height, line_height));
	right_btn->setFixedSize(QSize(line_height, line_height));
	connect(right_btn, &QPushButton::clicked, [=]() {

		});
	add_remove_layout->addWidget(right_btn);

	container->setLayout(add_remove_layout);

	body_layout->addWidget(container);

	QPushButton* push_button = new QPushButton();


	current_height += container->height();
	current_height += body_layout->spacing();
}
