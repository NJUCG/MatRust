#include "NodeWidgetLayerConfigLabel.h"

NodeWidgetLayerConfigLabel::NodeWidgetLayerConfigLabel()
{
	init();
	addComponent();
}

void NodeWidgetLayerConfigLabel::on_streamed(void* v, int)
{
	streamed = true;
	if (v) {
		QString fv = (*(LayerConfig**)v)->layer_name;
		setText(fv);
	}
}

void NodeWidgetLayerConfigLabel::value_changed(void* v, int)
{
	if (v) {
		QString fv = (*(LayerConfig**)v)->layer_name;
		setText(fv);
	}
}

void NodeWidgetLayerConfigLabel::on_cutoff(void*, int)
{
	streamed = false;
	setText(default_msg);
}

void NodeWidgetLayerConfigLabel::set_node_data(NodeData<LayerConfig*>* node_data)
{
	this->node_data = node_data;
	if (*node_data->value) {
		setText((*node_data->value)->layer_name);
	}
}

void NodeWidgetLayerConfigLabel::init()
{
	setStyleSheet(CssLoader::load_css("node_widget_label_style.css"));
}

void NodeWidgetLayerConfigLabel::addComponent()
{
	setAlignment(Qt::AlignCenter);

	UIModel* model = UIModel::get();
	setFixedHeight(model->float_slider_height);
}
