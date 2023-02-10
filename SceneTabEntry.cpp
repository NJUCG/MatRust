#include "SceneTabEntry.h"


SceneTabEntry::SceneTabEntry(bool is_odd, int offset, string type, string name, SceneTabEntryResponder* responder)
{
	this->is_odd = is_odd;
	this->offset_in_pixel = offset * UIModel::get()->scene_entry_unit_offset;
	this->type = type;
	this->name = name;
	this->responder = responder;
	init();
	addComponent();
}

void SceneTabEntry::set_icon(string type)
{
	UIModel* model = UIModel::get();
	QPushButton* push_button = new QPushButton();
	push_button->setObjectName("icon");
	QIcon* icon = new QIcon((string("resources/ui/icons/") + type + string(".png")).c_str());
	push_button->setIcon(*icon);
	push_button->setIconSize(QSize(model->scene_entry_icon_width, model->scene_entry_icon_width));
	push_button->setFixedSize(QSize(model->scene_entry_icon_width, model->scene_entry_icon_width));

	icon_layout->addWidget(push_button);
}

void SceneTabEntry::set_name(string name)
{
	label->setText(name.c_str());
}

void SceneTabEntry::clear_focus()
{
	setStyleSheet(default_style.c_str());
}

void SceneTabEntry::init()
{
	if (is_odd) {
		default_style = "QWidget{background-color:#282828;color:white;}QPushButton{background-color:transparent;color:white;background - radius: 4px;color:white;}";
	}
	else {
		default_style = "QWidget{background-color:#2B2B2B;color:white;}QPushButton{background-color:transparent;color:white;background - radius: 4px;color:white;}";
	}
	setStyleSheet(default_style.c_str());
}

void SceneTabEntry::addComponent()
{
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);

	QWidget* space = new QWidget();
	space->setFixedWidth(offset_in_pixel);
	layout->addWidget(space);

	body = new QWidget();

	body_layout = new QHBoxLayout();
	body_layout->setContentsMargins(0, 0, 0, 0);
	body_layout->setSpacing(0);

	icon_container = new QWidget();

	icon_layout = new QVBoxLayout();
	icon_layout->setContentsMargins(0, 0, 0, 0);
	icon_layout->setSpacing(0);

	icon_container->setLayout(icon_layout);

	body_layout->addWidget(icon_container);

	label_container = new QWidget();
	label_layout = new QVBoxLayout();
	label_layout->setContentsMargins(0, 0, 0, 0);
	label_layout->setSpacing(0);
	label = new QLabel();
	label->setAlignment(Qt::AlignLeft);

	label_layout->addWidget(label);

	label_container->setLayout(label_layout);

	body_layout->addWidget(label_container);

	body_layout->addWidget(new QWidget());


	body->setLayout(body_layout);

	layout->addWidget(body);

	setLayout(layout);

	setFixedHeight(UIModel::get()->scene_entry_line_height);

	set_icon(type);
	set_name(name);
}

void SceneTabEntry::enterEvent(QEnterEvent*)
{
	if (!mouse_in) {
		setStyleSheet("QWidget{background-color:#464646;color:white;}QPushButton{background-color:transparent;color:white;background - radius: 4px;color:white;}");
		mouse_in = true;
	}
}

void SceneTabEntry::leaveEvent(QEvent*)
{
	if (mouse_in) {
		if (!selected) {
			setStyleSheet(default_style.c_str());
		}
		mouse_in = false;
	}
}

void SceneTabEntry::mousePressEvent(QMouseEvent*)
{
	if (selected) {
		selected = false;
	}
	else {
		selected = true;
		setStyleSheet("QWidget{background-color:#464646;color:white;}QPushButton{background-color:transparent;color:white;background - radius: 4px;color:white;}");
		responder->on_select(name);
	}
}
