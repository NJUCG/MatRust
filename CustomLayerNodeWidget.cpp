#include "CustomLayerNodeWidget.h"

CustomLayerNodeWidget::CustomLayerNodeWidget(NodeWidgetResponder* responder, int loc_x, int loc_y): NodeWidget(responder, loc_x, loc_y, false)
{
	init();
	addComponent();
}

void CustomLayerNodeWidget::value_changed(string tag, string text)
{
	if (tag == "生长方式") {
		if (text == "DPD") {

		}
	}
}

CustomLayerNodeWidget::~CustomLayerNodeWidget()
{
}

void CustomLayerNodeWidget::init()
{
	NodeWidget::init();
	prepare_arguments();
}

void CustomLayerNodeWidget::addComponent()
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
	CustomLayerNodeWidget::body();
	body_widget->setLayout(body_layout);
	top_layout->addWidget(body_widget);
	top_layout->addWidget(new QWidget());
	wrapper_widget->setLayout(top_layout);

	wrapper_layout->addWidget(wrapper_widget);

	setObjectName("outer");
	setLayout(wrapper_layout);
}

void CustomLayerNodeWidget::body()
{
	vector<string> item_list;
	item_list.push_back("DPD");
	item_list.push_back("Ballistic");

	new_line("growth alg", item_list);
	
	add_float_argument("r", false, false);
	add_float_argument("g", false, false);
	add_float_argument("b", false, false);

	add_float_argument("transmittance", false, false);

	new_label("output", true, false);
}

void CustomLayerNodeWidget::prepare_arguments()
{
	title = string("Custom Layer");
	title_color = string("background-color:#2B652B;");
}

