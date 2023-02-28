#include "NodeWidgetLabel.h"

NodeWidgetLabel::NodeWidgetLabel()
{
	init();
	addComponent();
}

void NodeWidgetLabel::on_streamed(void* v)
{
	string fv = *((string*)v);
	streamed = true;
	setText(fv.c_str());
}

void NodeWidgetLabel::value_changed(void* v)
{
	if (streamed) {
		string fv = *((string*)v);
		setText(fv.c_str());
	}
}

void NodeWidgetLabel::on_cutoff(void*)
{
	streamed = false;
	setText("");
}

void NodeWidgetLabel::set_node_data(NodeData<string>* node_data)
{
	this->node_data = node_data;
	setText(node_data->value->c_str());
}

void NodeWidgetLabel::init()
{
	setStyleSheet(CssLoader::load_css("node_widget_label_style.css"));
}

void NodeWidgetLabel::addComponent()
{
	setAlignment(Qt::AlignCenter);

	UIModel* model = UIModel::get();
	setFixedHeight(model->float_slider_height);
}
