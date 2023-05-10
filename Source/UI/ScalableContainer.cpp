#include "ScalableContainer.h"

ScalableContainer::ScalableContainer(QWidget* content)
{
	this->content = content;
	init();
	addComponent();
}

void ScalableContainer::mouseMoveEvent(QMouseEvent* e)
{
	if (stretching) {
		delta_y = (e->pos() - prev_pos).y();
		prev_pos = e->pos();

		setFixedHeight(height() + delta_y);
	}
}

void ScalableContainer::mousePressEvent(QMouseEvent* e)
{
	prev_pos = e->pos();
	
	QPoint top_left = pull_btn->pos();
	QRect rect;
	rect.setTopLeft(top_left);
	rect.setWidth(pull_btn->width());
	rect.setHeight(pull_btn->height());

	if (rect.contains(prev_pos)) {
		stretching = true;
	}
}

void ScalableContainer::mouseReleaseEvent(QMouseEvent* e)
{
	stretching = false;
}

void ScalableContainer::init()
{
}

void ScalableContainer::addComponent()
{
	QVBoxLayout* top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(0);

	top_layout->addWidget(content);

	pull_btn = new QLabel();
	pull_btn->setAlignment(Qt::AlignCenter);
	pull_btn->setFixedHeight(20);
	pull_btn->setPixmap(QPixmap("resources/ui/icons/hanger-30.png"));

	top_layout->addWidget(pull_btn);

	setLayout(top_layout);
}
