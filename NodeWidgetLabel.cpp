#include "NodeWidgetLabel.h"

NodeWidgetLabel::NodeWidgetLabel()
{
	init();
	addComponent();
}

void NodeWidgetLabel::on_streamed(void* v,int)
{
	QString fv = *((QString*)v);
	streamed = true;
	setText(fv);
}

void NodeWidgetLabel::value_changed(void* v, int)
{
	QString fv = *((QString*)v);
	setText(fv);
}

void NodeWidgetLabel::on_cutoff(void*,int)
{
	streamed = false;
	setText(default_msg);
}

void NodeWidgetLabel::set_node_data(NodeData<QString>* node_data)
{
	this->node_data = node_data;
	setText(*(node_data->value));
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
