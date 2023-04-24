#include "NodeLineEditor.h"

NodeWidgetLineEditor::NodeWidgetLineEditor()
{
	init();
	addComponent();
}

void NodeWidgetLineEditor::on_streamed(void* v,int)
{
	QString fv = *((QString*)v);
	streamed = true;
	setText(fv);
	setEnabled(false);
}

void NodeWidgetLineEditor::value_changed(void* v, int)
{
	QString fv = *((QString*)v);
	setText(fv);
}


void NodeWidgetLineEditor::on_cutoff(void* v,int)
{
	streamed = false;
	setEnabled(true);
	setText(default_msg);
}

void NodeWidgetLineEditor::set_node_data(NodeData<QString>* data)
{
	node_data = data;
	setText(*(node_data->value));
}

void NodeWidgetLineEditor::init()
{
	setStyleSheet(CssLoader::load_css("node_widget_line_editor_style.css"));
}

void NodeWidgetLineEditor::addComponent()
{
	setAlignment(Qt::AlignCenter);
	UIModel* model = UIModel::get();
	setFixedHeight(model->float_slider_height);
	connect(this, &QLineEdit::editingFinished, [this]() {
		node_data->set(text());
		clearFocus();
		});
}
