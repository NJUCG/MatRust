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

NodeWidget::NodeWidget(NodeWidgetResponder* responder, QJsonObject obj, int loc_x, int loc_y)
{
	this->responder = responder;
	this->loc_x = loc_x;
	this->loc_y = loc_y;
}

void NodeWidget::value_changed(QString, QString)
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

int NodeWidget::get_node_data(string tag)
{
	if (data_cache.find(tag) == data_cache.end()) {
		return -1;
	}
	return data_cache[tag];
}

void NodeWidget::read(const QJsonObject& obj)
{
	type = obj["type"].toString();
}

void NodeWidget::write(QJsonObject& json)
{
	json["loc_x"] = loc_x;
	json["loc_y"] = loc_y;
}

void NodeWidget::destroy_widget()
{
	if (responder) {
		for (NodeDataButton* btn : buttons) {
			if (btn->is_linked) {
				responder->remove_connection(btn);
			}
		}
	}
	responder->on_widget_deleted(this);
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
	title = "环境vs";
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
	QHBoxLayout* header_layout = new QHBoxLayout();
	header_layout->setContentsMargins(5, 0, 0, 0);

	QLabel* value_label = new QLabel();
	value_label->setObjectName("header_label");
	value_label->setText(title);
	header_layout->addWidget(value_label);

	header_layout->addWidget(new QWidget());

	if (type != "layer_machine") {
		QPushButton* del_btn = new QPushButton();
		del_btn->setFixedSize(15, 15);
		del_btn->setIcon(QIcon("resources/ui/icons/delete-30.png"));
		del_btn->setIconSize(QSize(15, 15));

		connect(del_btn, &QPushButton::clicked, [=]() {
			destroy_widget();
			});

		header_layout->addWidget(del_btn);
	}

	header_widget->setLayout(header_layout);
	top_layout->addWidget(header_widget);
}

void NodeWidget::body()
{
}

int NodeWidget::new_float_argument(QString title, string tag, bool has_source, bool has_adapter, float min, float max, float delta)
{
	QWidget* line = new QWidget();
	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);
	int t = pool->creat<float>();
	FloatSliderBar* slider = new FloatSliderBar(nullptr, title, min, max, delta);

	pool->get_data<float>(t)->add_responder(slider);
	slider->set_node_data(pool->get_data<float>(t));

	line_layout->addWidget(slider);
	
	line->setLayout(line_layout);
	body_layout->addWidget(line);
	data_cache[tag] = t;
	connect(slider, &QSlider::valueChanged, [this, slider,t]() -> void{
		NodeData<float>* data = pool->get_data<float>(t);
		data->set(slider->d_value());
	});

	add_linker(line, t, has_source, has_adapter, slider->height());
	return t;
}

int NodeWidget::new_line(QString title, string tag, vector<QString> item_list,bool has_source,bool has_adapter)
{
	QWidget* line = new QWidget();
	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);

	int t = pool->creat<QString>();

	LabeledComboBox* combo = new LabeledComboBox(this, title);
	pool->get_data<QString>(t)->add_responder(combo);
	combo->set_node_data(pool->get_data<QString>(t));
	for (int i = 0; i < item_list.size(); i++) {
		combo->add_item(item_list[i]);
	}

	line_layout->addWidget(combo);
	line->setLayout(line_layout);
	body_layout->addWidget(line);
	data_cache[tag] = t;

	add_linker(line, t, has_source, has_adapter, combo->height());
	return t;
}

int NodeWidget::new_label(QString init_text, string tag, bool has_source, bool has_adapter, bool tagged, QString label_tag,int tag_width)
{
	QWidget* line = new QWidget();
	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);
	line_layout->setSpacing(5);

	int t = pool->creat<QString>();
	pool->set_value<QString>(t, init_text);

	if (tagged) {
		QLabel* tag_label = new QLabel();
		tag_label->setObjectName("tag");
		tag_label->setStyleSheet(CssLoader::load_css("node_widget_label_style.css"));
		tag_label->setText(label_tag);
		tag_label->setContentsMargins(5, 0, 5, 0);
		tag_label->setMinimumWidth(tag_width);
		tag_label->setAlignment(Qt::AlignCenter);
		line_layout->addWidget(tag_label);
	}

	NodeWidgetLabel* label = new NodeWidgetLabel();
	label->setText(init_text);
	label->default_msg = init_text;
	pool->get_data<QString>(t)->add_responder(label);
	label->set_node_data(pool->get_data<QString>(t));

	line_layout->addWidget(label);
	line->setLayout(line_layout);
	body_layout->addWidget(line);
	data_cache[tag] = t;

	add_linker(line, t, has_source, has_adapter, label->height());
	return t;
}

int NodeWidget::new_line_edit(QString init_text, string tag, bool has_source, bool has_adapter, bool tagged, QString label_tag, int tag_width)
{
	QWidget* line = new QWidget();
	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);
	line_layout->setSpacing(5);

	int t = pool->creat<QString>();
	pool->set_value<QString>(t, init_text);

	if (tagged) {
		QLabel* tag_label = new QLabel();
		tag_label->setObjectName("tag");
		tag_label->setStyleSheet(CssLoader::load_css("node_widget_label_style.css"));
		tag_label->setText(label_tag);
		tag_label->setContentsMargins(5, 0, 5, 0);
		tag_label->setMinimumWidth(tag_width);
		tag_label->setAlignment(Qt::AlignCenter);
		line_layout->addWidget(tag_label);
	}

	NodeWidgetLineEditor* editor = new NodeWidgetLineEditor();
	editor->setText(init_text);
	editor->default_msg = init_text;
	pool->get_data<QString>(t)->add_responder(editor);
	editor->set_node_data(pool->get_data<QString>(t));

	line_layout->addWidget(editor);
	line->setLayout(line_layout);
	body_layout->addWidget(line);
	data_cache[tag] = t;

	add_linker(line, t, has_source, has_adapter, editor->height());
	return t;
}

int NodeWidget::new_palette(string tag, bool interactable, bool has_source, bool has_adapter, bool tagged, QString label_tag, int tag_width)
{
	QWidget* line = new QWidget();
	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);
	line_layout->setSpacing(5);

	int t = pool->creat<QColor>();
	pool->set_value<QColor>(t, QColor(0, 0, 0));

	if (tagged) {
		QLabel* tag_label = new QLabel();
		tag_label->setObjectName("tag");
		tag_label->setStyleSheet(CssLoader::load_css("node_widget_label_style.css"));
		tag_label->setText(label_tag);
		tag_label->setContentsMargins(5, 0, 5, 0);
		tag_label->setMinimumWidth(tag_width);
		tag_label->setAlignment(Qt::AlignCenter);
		line_layout->addWidget(tag_label);
	}

	NodeWidgetPalette* palette = new NodeWidgetPalette();
	if (!interactable) {
		palette->set_interactable(false);
	}
	pool->get_data<QColor>(t)->add_responder(palette);
	palette->set_node_data(pool->get_data<QColor>(t));

	line_layout->addWidget(palette);
	line->setLayout(line_layout);
	body_layout->addWidget(line);
	data_cache[tag] = t;

	add_linker(line, t, has_source, has_adapter, palette->height());
	return t;
}

int NodeWidget::new_layer_config(LayerConfig* config, string tag, int name_data_t, bool has_source, bool tagged, QString label_tag, int tag_width)
{
	QWidget* line = new QWidget();
	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);
	line_layout->setSpacing(5);

	int t = pool->creat<LayerConfig*>();
	pool->set_value<LayerConfig*>(t, config);

	LayerConfig* tv = pool->get_value<LayerConfig*>(t);

	if (tagged) {
		QLabel* tag_label = new QLabel();
		tag_label->setObjectName("tag");
		tag_label->setStyleSheet(CssLoader::load_css("node_widget_label_style.css"));
		tag_label->setText(label_tag);
		tag_label->setContentsMargins(5, 0, 5, 0);
		tag_label->setMinimumWidth(tag_width);
		tag_label->setAlignment(Qt::AlignCenter);
		line_layout->addWidget(tag_label);
	}

	NodeWidgetLabel* label = new NodeWidgetLabel();
	label->setText(config->layer_name);
	label->default_msg = config->layer_name;
	pool->get_data<QString>(name_data_t)->add_responder(label);
	label->set_node_data(pool->get_data<QString>(name_data_t));

	line_layout->addWidget(label);
	line->setLayout(line_layout);
	body_layout->addWidget(line);
	data_cache[tag] = t;

	add_linker(line, t, has_source, false, label->height());
	return t;
}

int NodeWidget::new_layer_config(LayerConfig* c, string tag, bool has_source, bool has_adapter, bool tagged, QString label_tag, int tag_width)
{
	QWidget* line = new QWidget();
	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);
	line_layout->setSpacing(5);

	int t = pool->creat<LayerConfig*>();
	pool->set_value<LayerConfig*>(t, c);

	if (tagged) {
		QLabel* tag_label = new QLabel();
		tag_label->setObjectName("tag");
		tag_label->setStyleSheet(CssLoader::load_css("node_widget_label_style.css"));
		tag_label->setText(label_tag);
		tag_label->setContentsMargins(5, 0, 5, 0);
		tag_label->setMinimumWidth(tag_width);
		tag_label->setAlignment(Qt::AlignCenter);
		line_layout->addWidget(tag_label);
	}

	NodeWidgetLayerConfigLabel* label = new NodeWidgetLayerConfigLabel();
	label->default_msg = NODE_EDITOR_NODE_WIDGET_ENTRY_EMPTY_SLOT;
	label->setText(NODE_EDITOR_NODE_WIDGET_ENTRY_EMPTY_SLOT);
	pool->get_data<LayerConfig*>(t)->add_responder(label);
	label->set_node_data(pool->get_data<LayerConfig*>(t));

	line_layout->addWidget(label);
	line->setLayout(line_layout);
	body_layout->addWidget(line);
	data_cache[tag] = t;

	add_linker(line, t, has_source, has_adapter, label->height());
	return t;
}

void NodeWidget::add_linker(QWidget* line, int t, bool has_source, bool has_adapter, int height)
{
	if (has_adapter) {
		QRect rect = line->contentsRect();
		QPoint l = line->pos();
		int t0 = rect.topLeft().y();
		t0 = rect.height();
		int y = rect.topLeft().y() + rect.height() / 2;
		NodeDataButton* adapter_btn = new NodeDataButton(this, pool, t, true);
		adapter_btn->setParent(this);
		adapter_btn->move(0, current_height + height / 2 - model->node_button_width / 2);
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
		source_btn->move(width() - model->node_button_width, current_height + height / 2 - model->node_button_width / 2);
		wrapper_widget->stackUnder(source_btn);
		source_btn->button_index = buttons.size();
		source_btn->button_index = buttons.size();
		buttons.push_back(source_btn);
	}
	current_height += height;
	current_height += body_layout->spacing();
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
			prev_position = start_position;
		}
	}
	QWidget::mousePressEvent(e);
}

void NodeWidget::mouseMoveEvent(QMouseEvent* e)
{
	if (e->buttons() & Qt::LeftButton) {
		if (dragging) {
			// 自开始移动的变化
			QPoint delta = e->globalPos() - start_position;
			move(frame_position + delta);
			// 自上一次该函数调用的变化
			QPoint t_delta = e->globalPos() - prev_position;
			loc_x += t_delta.x();
			loc_y += t_delta.y();
			prev_position = e->globalPos();
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

void NodeWidget::remove_connection(int id)
{
	if (id >= 0 && id < buttons.size())
		responder->remove_connection(buttons[id]);
}
