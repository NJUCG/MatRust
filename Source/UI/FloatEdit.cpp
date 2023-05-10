#include "FloatEdit.h"

FloatEdit::FloatEdit()
{
	init();
	addComponent();
}

void FloatEdit::set_min_value(float m)
{
	min_value = m;
	if (value < min_value) {
		value = m;
	}
}

void FloatEdit::set_max_value(float m)
{
	max_value = m;
	if (max_value < value) {
		value = m;
	}
}

void FloatEdit::init()
{
	current_state = EditorState::IDLE;
	value = 0;
	min_value = 0;
	max_value = 100;
	step = 0.1f;
	sensitivity = 1.0f;
	setMouseTracking(false);
	setStyleSheet(CssLoader::load_css("float_edit_idle.css"));
	pages = vector<QWidget*>();
}

inline void FloatEdit::set_value(float new_value, bool respond)
{
	if (new_value < min_value) {
		value = min_value;
	}else if(max_value<new_value){
		value = max_value;
	}
	else {
		value = new_value;
	}
	middle_button->setText(get_value().c_str());
	if (responder && respond) {
		responder->on_value_changed(name, value);
	}
	update();
}

void FloatEdit::set_unit(string unit)
{
	this->unit = unit;
	value_label->setText(get_value().c_str());
	middle_button->setText(get_value().c_str());
}

void FloatEdit::addComponent()
{
	setObjectName("top");

	hover_mode = new QWidget();
	hover_mode->setObjectName("hover_mode");

	QVBoxLayout* top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(0);

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	int height = UIModel::get()->float_edit_height;

	int icon_width = 7;
	left_button = new FloatEditPushButton(this);
	left_button->setIcon(QIcon("resources/ui/icons/left_arrow-100.png"));
	left_button->setIconSize(QSize(icon_width, icon_width));
	left_button->setFixedWidth(UIModel::get()->float_edit_arrow_width);
	left_button->setObjectName("left_button");
	left_button->setFixedHeight(height);
	left_button->setStyleSheet(CssLoader::load_css("float_edit_left_btn_style.css"));
	connect(left_button, &QPushButton::clicked, [this]()->void {
		set_value(value - step);
	});

	layout->addWidget(left_button);
	
	middle_button = new FloatEditMiddleButton(this);
	middle_button->setText(get_value().c_str());
	middle_button->setObjectName("middle_button");
	middle_button->setFixedHeight(height);
	middle_button->setStyleSheet("border-radius:0px");

	layout->addWidget(middle_button);

	right_button = new FloatEditPushButton(this);
	right_button->setFixedWidth(UIModel::get()->float_edit_arrow_width);
	right_button->setIcon(QIcon("resources/ui/icons/right_arrow-100.png"));
	right_button->setIconSize(QSize(icon_width, icon_width));
	right_button->setObjectName("right_button");
	right_button->setFixedHeight(height);
	right_button->setStyleSheet(CssLoader::load_css("float_edit_right_btn_style.css"));
	layout->addWidget(right_button);
	connect(right_button, &QPushButton::clicked, [this]()->void {
		set_value(value + step);
	});

	hover_mode->setLayout(layout);

	hover_mode->setVisible(false);

	top_layout->addWidget(hover_mode);

	pages.push_back(hover_mode);

	value_label = new FloatEditLabel(this);
	value_label->setText(get_value().c_str());
	value_label->setAlignment(Qt::AlignCenter);
	value_label->setFixedHeight(height);
	top_layout->addWidget(value_label);

	pages.push_back(value_label);

	editor = new FloatEditLineEdit(this, height);
	editor->setText(get_value_without_unit().c_str());
	editor->setAlignment(Qt::AlignLeft);
	editor->setFixedHeight(height);
	connect(editor, &QLineEdit::editingFinished, [this]()->void {
		edit_finish_edit();
	});
	
	editor->setVisible(false);
	editor->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	top_layout->addWidget(editor);

	pages.push_back(editor);

	current_page = value_label;

	hover_mode->blockSignals(true);
	editor->blockSignals(true);

	//setCurrentIndex(1);

	setLayout(top_layout);
}

void FloatEdit::switchTo(EditorState state)
{
	if (state == IDLE) {

	}
	else if(state == HOVER){

	}
	else if(state == EDIT) {

	}
}

void FloatEdit::enterEvent(QEnterEvent* event)
{

}

void FloatEdit::leaveEvent(QEvent* event)
{
	if (current_state == HOVER) {
		current_state = IDLE;
		setCurrentIndex(1);
		hover_mode->blockSignals(true);
		editor->blockSignals(true);
		value_label->blockSignals(false);
	}
}

void FloatEdit::mouseMoveEvent(QMouseEvent* e)
{
	if (!draging) {
		draging = true;
		cursor_x = e->pos().x();
	}
	int new_x = e->pos().x();
	set_value(value + (new_x - cursor_x) * sensitivity);
	cursor_x = new_x;
}


void FloatEdit::middle_button_mouse_move(QMouseEvent* e)
{
	mouseMoveEvent(e);
}

void FloatEdit::middle_button_mouse_enter(QEnterEvent* e)
{
	enterEvent(e);
	middle_button->setStyleSheet("background-color:#797979;border-radius:0px;");
	if (current_state == IDLE) {
		current_state = HOVER;
		setCurrentIndex(0);
		value_label->blockSignals(true);
		editor->blockSignals(true);
		hover_mode->blockSignals(false);
	}
}

void FloatEdit::middle_button_mouse_release(QMouseEvent* e)
{
	value_label->setText(get_value().c_str());
	if (current_state == HOVER && draging == false) {
		current_state = EDIT;
		setCurrentIndex(2);
		hover_mode->blockSignals(true);
		value_label->blockSignals(true);
		editor->blockSignals(false);
	}
	if (draging) {
		draging = false;
	}
}

void FloatEdit::middle_button_mouse_leave(QEvent*)
{
	middle_button->setStyleSheet("background-color:#545454;border-radius:0px;");
	if (current_state == HOVER) {
		setCurrentIndex(1);
		current_state = IDLE;
		value_label->setText(get_value().c_str());
		hover_mode->blockSignals(true);
		editor->blockSignals(true);
		value_label->blockSignals(false);
	}
	from_middle = true;
}

void FloatEdit::middle_button_mouse_press(QMouseEvent*)
{
	
}

void FloatEdit::label_mouse_enter(QEnterEvent*)
{
	if (current_state == IDLE) {
		setCurrentIndex(0);
		current_state = HOVER;
		value_label->blockSignals(true);
		editor->blockSignals(true);
		hover_mode->blockSignals(false);
	}
	from_button = false;
}

void FloatEdit::pushbutton_mouse_leave(QEvent* e)
{
	if (current_state == HOVER) {
		setCurrentIndex(1);
		current_state = IDLE;
		value_label->setText(get_value().c_str());
		hover_mode->blockSignals(true);
		editor->blockSignals(true);
		value_label->blockSignals(false);
	}
}

void FloatEdit::pushbutton_mouse_enter(QEnterEvent* e)
{
	if (current_state == IDLE) {
		setCurrentIndex(0);
		current_state = HOVER;
		value_label->setText(get_value().c_str());
		editor->blockSignals(true);
		value_label->blockSignals(true);
		hover_mode->blockSignals(false);
	}
}

void FloatEdit::edit_mouse_leave(QEvent* e)
{

}

void FloatEdit::edit_finish_edit()
{
	if (current_state == EDIT) {
		float temp = 0;
		stringstream es(editor->text().toStdString());
		if (es >> temp) {
			set_value(temp);
		}
		setCurrentIndex(0);
		current_state = HOVER;
		value_label->setText(get_value().c_str());
		middle_button->setText(get_value().c_str());
		value_label->blockSignals(true);
		editor->blockSignals(true);
		hover_mode->blockSignals(false);
	}
}

string FloatEdit::get_value()
{
	stringstream ss;
	ss << std::setiosflags(std::ios::fixed) << std::setprecision(2) << value;
	return string(ss.str() + unit);
}

void FloatEdit::setCurrentIndex(int idx)
{
	current_page->setVisible(false);
	current_page = pages[idx];
	current_page->setVisible(true);
}

void FloatEdit::resizeEvent(QResizeEvent* e)
{
	editor->w = width();
	QWidget::resizeEvent(e);

}

QSize FloatEdit::minimumSizeHint() const
{
	QSize s = QWidget::minimumSizeHint();
	return s;
}

QSize FloatEdit::sizeHint() const
{
	QSize s = QWidget::sizeHint();
	return s;
}


string FloatEdit::get_value_without_unit()
{
	stringstream ss;
	ss << std::setiosflags(std::ios::fixed) << std::setprecision(2) << value;
	return string(ss.str());
}
