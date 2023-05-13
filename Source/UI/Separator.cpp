#include "Separator.h"

Separator::Separator(QWidget* positive, QWidget* negative, bool x_dir)
{
	init(positive, negative, x_dir);
}

Separator::Separator()
{
}

void Separator::init(QWidget* positive, QWidget* negative,bool x_dir)
{
	this->positive = positive;
	this->negative = negative;

	this->x_dir = x_dir;

	int w = 4;

	setStyleSheet("background-color:black;");

	if (x_dir) {
		positive_min = positive->minimumWidth();
		negative_min = negative->minimumWidth();

		setFixedWidth(w);
	}
	else {
		positive_min = positive->minimumHeight();
		negative_min = negative->minimumHeight();

		setFixedHeight(w);
	}

	this->negative_min = negative_min;
}

void Separator::mouseMoveEvent(QMouseEvent* e)
{
	if (in) {
		float delta = -1;
		if (x_dir) {
			delta = e->globalPos().x() - before_p;
		}
		else {
			delta = e->globalPos().y() - before_p;
		}
		if (x_dir) {
			int p = positive->width() + delta;
			int n = negative->width() - delta;
			if (p   >= positive_min && n  >= negative_min) {
				positive->setFixedWidth(p);
				negative->setFixedWidth(n);
			}
			before_p = e->globalPos().x();
		}
		else {
			int p = positive->height() + delta;
			int n = negative->height() - delta;
			if (p >= positive_min && n >= negative_min) {
				positive->setFixedHeight(p);
				negative->setFixedHeight(n);
			}
			before_p = e->globalPos().y();
		}
	}
}

void Separator::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton) {
		in = true;
		if (x_dir) {
			before_p = e->globalPos().x();
		}
		else {
			before_p = e->globalPos().y();
		}
	}
}

void Separator::mouseReleaseEvent(QMouseEvent* e)
{
	in = false;
}

void Separator::enterEvent(QEnterEvent*)
{
	if (x_dir) {
		setCursor(Qt::SizeHorCursor);
	}
	else {
		setCursor(Qt::SizeVerCursor);
	}
}

void Separator::leaveEvent(QEvent*)
{
	setCursor(Qt::ArrowCursor);
}
