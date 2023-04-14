#include "FileSelector.h"

FileSelector::FileSelector()
{
	init();
	addComponent();
}

void FileSelector::set_name(QString name)
{
	if (name_tag) {
		name_tag->setText(name);
	}
}

void FileSelector::set_normal(QString name)
{
	if (normal_label) {
		normal_label->setText(name);
	}
}

void FileSelector::init()
{
	setStyleSheet(CssLoader::load_css("file_selector_style.css"));
}

void FileSelector::addComponent()
{
	top_layout = new QHBoxLayout();

	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(5);

	name_tag = new QLabel();
	name_tag->setObjectName("name_tag");
	name_tag->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	name_tag->setText("");

	top_layout->addWidget(name_tag);

	normal_label = new QLabel();
	normal_label->setObjectName("normal_label");
	normal_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	normal_label->setText("");

	top_layout->addWidget(normal_label);

	setLayout(top_layout);

	setFixedHeight(UIModel::get()->selector_height);
	setFixedWidth(UIModel::get()->selector_width);
}

void FileSelector::mousePressEvent(QMouseEvent* e)
{
	QPoint topLeft = normal_label->mapToGlobal(QPoint(0, 0));
	QRect rect;
	rect.setTopLeft(topLeft);
	rect.setSize(normal_label->size());
	if (rect.contains(e->globalPos())) {
		QString* t = new QString();
		*t = QFileDialog::getOpenFileName(nullptr, OJBECT_TAB_SELECT_MODEL, ".", "*.obj");
		normal_label->setText(*t);
		if (event_name != "") {
			EventAdapter::shared->push_data(t);
			EventAdapter::shared->trigger_event(event_name.c_str());
		}
		if (responder) {
			responder->on_select_file(tag, t->toStdString());
		}
	}
}
