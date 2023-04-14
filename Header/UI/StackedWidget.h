#pragma once
#include<qstackedwidget.h>
#include<qdebug.h>
class StackedWidget: public QStackedWidget
{
public:
	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;
};

