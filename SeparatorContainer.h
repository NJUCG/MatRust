#pragma once
#include<qwidget.h>
class SeparatorContainer: public QWidget
{
public:
	SeparatorContainer();
	void addContainer(QWidget*,int bw = QWIDGETSIZE_MAX,int bh = QWIDGETSIZE_MAX);
	void addContainer(QWidget*, QSize);
protected:
	QList<QWidget*> containers;
	QList<QSize> base_max;
	void resizeEvent(QResizeEvent*) override;
};

