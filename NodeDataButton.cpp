#include "NodeDataButton.h"

NodeDataButton::NodeDataButton(NodeDataButtonResponder* responder,NodePool* pool, int id,bool is_adapter, string color)
{
	this->responder = responder;
	this->pool = pool;
	this->id = id;
	this->is_adapter = is_adapter;
	setObjectName(color.c_str());
	init();
	addComponent();
}

void NodeDataButton::init()
{
	setStyleSheet(CssLoader::load_css("node_data_button.css"));
}

void NodeDataButton::addComponent()
{
	int radius = UIModel::get()->node_button_width;
	setFixedSize(radius, radius);
	connect(this, &QPushButton::clicked, this, &NodeDataButton::on_click);
}

void NodeDataButton::on_click()
{
}

void NodeDataButton::mousePressEvent(QMouseEvent* e)
{
	if (!is_linked) {
		is_linking = true;
		if (responder)
			responder->start_linking(button_index);
	}
}

void NodeDataButton::mouseReleaseEvent(QMouseEvent* e)
{
	if (!is_linked) {
		is_linking = false;
		if (responder)
			responder->release_on_me(e->globalPos());
	}
	else {
		if (responder)
			responder->remove_connection(button_index);
	}
}

void NodeDataButton::mouseMoveEvent(QMouseEvent* e)
{
	if (is_linking) {
		responder->update_temp_curve(e->globalPos());
	}
}

QPoint NodeDataButton::center()
{
	return mapToGlobal(QPoint(width() / 2, height() / 2));
}
