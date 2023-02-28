#include "NodeWidget.h"

NodeWidget::NodeWidget(NodeWidgetResponder* responder,int loc_x,int loc_y, bool father)
{
	this->responder = responder;
	pool = responder->get_pool();
	this->loc_x = loc_x;
	this->loc_y = loc_y;
	if (father) {
		init();
		addComponent();
	}
}

void NodeWidget::value_changed(string, string)
{
}

NodeDataButton* NodeWidget::release_in_me(QPoint pos)
{
	int pl = buttons.size();
	for (int i = 0; i < pl; i++) {
		NodeDataButton* t = buttons[i];
		QPoint topLeft = t->mapToGlobal(QPoint(0, 0));
		QRect rect = QRect();
		rect.setTopLeft(topLeft);
		rect.setSize(t->contentsRect().size());
		if (rect.contains(pos)) {
			return t;
		}
	}
	return nullptr;
}

void NodeWidget::update_temp_curve(QPoint pos)
{
	responder->update_temp_curve(pos);
}

void NodeWidget::init()
{
	current_height = 0;
	model = UIModel::get();
	data_cache = unordered_map<string, int>();
	buttons = vector<NodeDataButton*>();
	id_generator = new IdGenerator(time(NULL), 20);
	setFixedWidth(model->node_widget_width);
	setStyleSheet(CssLoader::load_css("node_widget.css"));
	prepare_arguments();
}

void NodeWidget::addComponent()
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
	body();
	body_widget->setLayout(body_layout);
	top_layout->addWidget(body_widget);
	top_layout->addWidget(new QWidget());
	wrapper_widget->setLayout(top_layout);

	wrapper_layout->addWidget(wrapper_widget);

	setObjectName("outer");
	setLayout(wrapper_layout);
}

void NodeWidget::prepare_arguments()
{
	title = string("»·¾³vs");
	title_color = string("background-color:#2B652B;");
	// #2B652B;
	// #3C1D26;
}

void NodeWidget::header()
{
	header_widget = new QWidget();
	header_widget->setFixedHeight(model->node_widget_header_height);
	header_widget->setStyleSheet(title_color.c_str());
	header_widget->setObjectName("header_widget");
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(5, 0, 0, 0);

	QLabel* value_label = new QLabel();
	value_label->setObjectName("header_label");
	value_label->setText(QString::fromLocal8Bit(title.c_str()));
	layout->addWidget(value_label);

	header_widget->setLayout(layout);
	top_layout->addWidget(header_widget);
}

void NodeWidget::body()
{
	add_float_argument("test", true, true);
	add_float_argument("test", true, true);
}

int NodeWidget::add_float_argument(string title, bool has_source, bool has_adapter)
{
	QWidget* line = new QWidget();
	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);
	int t = pool->creat<float>();
	FloatSliderBar* slider = new FloatSliderBar(nullptr, title, 0, 100);

	pool->get_data<float>(t)->add_responder(slider);
	slider->set_node_data(pool->get_data<float>(t));

	line_layout->addWidget(slider);
	
	line->setLayout(line_layout);
	body_layout->addWidget(line);
	data_cache[title] = t;
	connect(slider, &QSlider::valueChanged, [this, slider,t]() -> void{
		NodeData<float>* data = pool->get_data<float>(t);
		data->set(slider->d_value());
	});

	if (has_adapter) {
		QRect rect = line->contentsRect();
		QPoint l = line->pos();
		int t0 = rect.topLeft().y();
		t0 = rect.height();
		int y = rect.topLeft().y() + rect.height() / 2;
		NodeDataButton* adapter_btn = new NodeDataButton(this, pool, t, true);
		adapter_btn->setParent(this);
		adapter_btn->move(0, current_height + slider->height() / 2 - model->node_button_width / 2);
		wrapper_widget->stackUnder(adapter_btn);
		adapter_btn->button_index = buttons.size();
		buttons.push_back(adapter_btn);
	}
	if (has_source) {
		QRect rect = line->frameGeometry();
		QPoint l = line->pos();
		int y = rect.topLeft().y() + rect.height() / 2;
		NodeDataButton* source_btn = new NodeDataButton(this, pool, t, false);
		source_btn->setParent(this);
		int t = width();
		source_btn->move(width() - model->node_button_width, current_height + slider->height() / 2 - model->node_button_width / 2);
		wrapper_widget->stackUnder(source_btn);
		source_btn->button_index = buttons.size();
		buttons.push_back(source_btn);
	}
	current_height += slider->height();
	current_height += body_layout->spacing();
	return t;
}

int NodeWidget::new_line(string title, vector<string> item_list,bool has_source,bool has_adapter)
{
	QWidget* line = new QWidget();
	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);

	int t = pool->creat<string>();

	LabeledComboBox* combo = new LabeledComboBox(this, title);
	pool->get_data<string>(t)->add_responder(combo);
	combo->set_node_data(pool->get_data<string>(t));
	for (int i = 0; i < item_list.size(); i++) {
		combo->add_item(item_list[i]);
	}

	line_layout->addWidget(combo);
	line->setLayout(line_layout);
	body_layout->addWidget(line);
	data_cache[title] = t;

	if (has_adapter) {
		QRect rect = line->contentsRect();
		QPoint l = line->pos();
		int t0 = rect.topLeft().y();
		t0 = rect.height();
		int y = rect.topLeft().y() + rect.height() / 2;
		NodeDataButton* adapter_btn = new NodeDataButton(this, pool, t, true);
		adapter_btn->setParent(this);
		adapter_btn->move(0, current_height + combo->height() / 2 - model->node_button_width / 2);
		adapter_btn->show();
		wrapper_widget->stackUnder(adapter_btn);
		adapter_btn->button_index = buttons.size();
		buttons.push_back(adapter_btn);
	}
	if (has_source) {
		QRect rect = line->frameGeometry();
		QPoint l = line->pos();
		int y = rect.topLeft().y() + rect.height() / 2;
		NodeDataButton* source_btn = new NodeDataButton(this, pool, t, false);
		source_btn->setParent(this);
		int t = width();
		source_btn->move(width() - model->node_button_width, current_height + combo->height() / 2 - model->node_button_width / 2);
		wrapper_widget->stackUnder(source_btn);
		source_btn->button_index = buttons.size();
		source_btn->show();
		buttons.push_back(source_btn);
	}
	current_height += combo->height();
	current_height += body_layout->spacing();
	return t;
}

int NodeWidget::new_label(string init_text, bool has_source, bool has_adapter)
{
	QWidget* line = new QWidget();
	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);

	int t = pool->creat<string>();
	pool->set_value<string>(t, init_text);

	NodeWidgetLabel* label = new NodeWidgetLabel();
	label->setText(QString::fromLocal8Bit(init_text.c_str()));
	pool->get_data<string>(t)->add_responder(label);
	label->set_node_data(pool->get_data<string>(t));

	line_layout->addWidget(label);
	line->setLayout(line_layout);
	body_layout->addWidget(line);
	data_cache[title] = t;

	if (has_adapter) {
		QRect rect = line->contentsRect();
		QPoint l = line->pos();
		int t0 = rect.topLeft().y();
		t0 = rect.height();
		int y = rect.topLeft().y() + rect.height() / 2;
		NodeDataButton* adapter_btn = new NodeDataButton(this, pool, t, true);
		adapter_btn->setParent(this);
		adapter_btn->move(0, current_height + label->height() / 2 - model->node_button_width / 2);
		wrapper_widget->stackUnder(adapter_btn);
		adapter_btn->button_index = buttons.size();
		buttons.push_back(adapter_btn);
	}
	if (has_source) {
		QRect rect = line->frameGeometry();
		QPoint l = line->pos();
		int y = rect.topLeft().y() + rect.height() / 2;
		NodeDataButton* source_btn = new NodeDataButton(this, pool, t, false);
		source_btn->setParent(this);
		int t = width();
		source_btn->move(width() - model->node_button_width, current_height + label->height() / 2 - model->node_button_width / 2);
		wrapper_widget->stackUnder(source_btn);
		source_btn->button_index = buttons.size();
		source_btn->button_index = buttons.size();
		buttons.push_back(source_btn);
	}
	current_height += label->height();
	current_height += body_layout->spacing();
	return t;
}

void NodeWidget::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton) {
		QRect rect = top_layout->contentsRect();
		rect.setBottom(rect.top() + model->node_widget_header_height);
		if (rect.contains(e->pos())) {
			dragging = true;
			start_position = e->globalPos();
			frame_position = frameGeometry().topLeft();
		}
	}
	QWidget::mousePressEvent(e);
}

void NodeWidget::mouseMoveEvent(QMouseEvent* e)
{
	if (e->buttons() & Qt::LeftButton) {
		if (dragging) {
			QPoint delta = e->globalPos() - start_position;
			move(frame_position + delta);
			responder->on_widget_move();
		}
	}
	QWidget::mouseMoveEvent(e);
}

void NodeWidget::mouseReleaseEvent(QMouseEvent* e)
{
	dragging = false;
	responder->reset_curving();
	QWidget::mouseReleaseEvent(e);
}

void NodeWidget::start_linking(int index)
{
	if (index >= 0 && index < buttons.size())
		responder->start_linking(buttons[index]);
}

void NodeWidget::release_on_me(QPoint point)
{
	responder->release_on_me(point);
}
