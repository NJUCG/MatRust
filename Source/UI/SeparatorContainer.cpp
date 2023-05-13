#include "SeparatorContainer.h"

SeparatorContainer::SeparatorContainer()
{
	containers = QList<QWidget*>();
	base_max = QList<QSize>();
}

void SeparatorContainer::addContainer(QWidget* w, int bw, int bh)
{
	containers.append(w);
	base_max.append(QSize(bw, bh));
}

void SeparatorContainer::addContainer(QWidget* w,QSize base)
{
	containers.append(w);
	base_max.append(base);
}

void SeparatorContainer::resizeEvent(QResizeEvent* e)
{
	QList<QWidget*> w_l= findChildren<QWidget*>();
	for (int i = 0; i < containers.size(); i++) {
		QWidget* w = containers.at(i);
		QSize s = base_max.at(i);
		//w->setMaximumSize(s);
	}
	QWidget::resizeEvent(e);
}
