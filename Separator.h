#pragma once
#include<qwidget.h>
#include<qevent.h>
class Separator: public QWidget
{
public:
	Separator(QWidget*, QWidget*, bool);
	Separator();
	void init(QWidget*, QWidget*, bool);
protected:
	QWidget* positive;
	QWidget* negative;
	bool x_dir;
	bool in = false;

	int positive_min;
	int negative_min;

	void mouseMoveEvent(QMouseEvent*) override;
	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void enterEvent(QEnterEvent*) override;
	void leaveEvent(QEvent*) override;

	float before_p = -1;
};

