#include "StretchContainer.h"

StretchContainer::StretchContainer(QWidget* left, QWidget* right, float left_s, float right_s,float spacing)
{
	this->left = left;
	this->right = right;
	this->total_s = left_s + right_s;
	this->left_s = left_s / total_s;
	this->right_s = right_s / total_s;
	this->spacing = spacing;

	QHBoxLayout* top_layout = new QHBoxLayout();
	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(spacing);

	setSizePolicy(QSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Preferred));

	top_layout->addWidget(left);
	top_layout->addWidget(right);

	setLayout(top_layout);

}

void StretchContainer::resizeEvent(QResizeEvent* e)
{
	QWidget::resizeEvent(e);
	int w = width() - spacing;
	left->setFixedWidth(left_s * w);
	right->setFixedWidth(right_s * w);
}
