#include "LayerMachineNodeWidget.h"

LayerMachineNodeWidget::LayerMachineNodeWidget(NodeWidgetResponder* responder, int loc_x, int loc_y) : NodeWidget(responder, loc_x, loc_y, false)
{
	init();
	addComponent();
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
	title = string("Layer Machine");
	title_color = string("background-color:#2B652B;");
}

void LayerMachineNodeWidget::body()
{
	new_label("0", false, true);
	new_label("1", false, true);
	new_label("2", false, true);
	new_label("3", false, true);
	new_label("4", false, true);
}

void LayerMachineNodeWidget::add_layer_slot()
{
}

void LayerMachineNodeWidget::remove_layer_slot()
{
}
