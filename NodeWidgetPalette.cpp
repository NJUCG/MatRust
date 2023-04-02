#include "NodeWidgetPalette.h"

NodeWidgetPalette::NodeWidgetPalette()
{
	init();
	addComponent();
}

NodeWidgetPalette::~NodeWidgetPalette()
{
	if (dialog) {
		delete dialog;
	}
}

void NodeWidgetPalette::on_streamed(void* v,int)
{
	current_color = *((QColor*)v);
	streamed = true;
	setEnabled(false);
	update_color();
}

void NodeWidgetPalette::value_changed(void* v, int)
{
	current_color = *((QColor*)v);
	update_color();
}

void NodeWidgetPalette::on_cutoff(void*,int)
{
	streamed = false;
	setEnabled(true && interactable);
	current_color.setRgb(0, 0, 0);
	update_color();
}

void NodeWidgetPalette::set_node_data(NodeData<QColor>* data)
{
	node_data = data;
	current_color = *(node_data->value);
	update_color();
}

void NodeWidgetPalette::set_interactable(bool i)
{
	interactable = i;
	if (!i) {
		setEnabled(false);
	}
}

void NodeWidgetPalette::init()
{
	current_color.setRgb(0, 0, 0);
}

void NodeWidgetPalette::addComponent()
{
	UIModel* model = UIModel::get();
	setFixedHeight(model->float_slider_height);

	dialog = new QColorDialog();

	connect(this, &QPushButton::clicked, [=]() {
		dialog->show();
		});

	connect(dialog, &QColorDialog::currentColorChanged, [=]() {
		current_color = dialog->currentColor();
		node_data->set(current_color);
		update_color();
		});

	update_color();
}

void NodeWidgetPalette::update_color()
{
	string color_string = "#" 
		+ to_hex_string(current_color.red())
		+ to_hex_string(current_color.green())
		+ to_hex_string(current_color.blue());
	string style = "QPushButton{border-radius:4px;background-color:" + color_string + ";" + "}";

	setStyleSheet(style.c_str());
}

string NodeWidgetPalette::to_hex_string(int i)
{
	std::ostringstream ss;
	ss << std::hex << i;
	return ss.str();
}
