#pragma once
#include<qwidget.h>
#include<qlayout.h>

class StretchContainer: public QWidget
{
public:
	StretchContainer(QWidget*,QWidget*,float,float,float spacing = 5);

	void resizeEvent(QResizeEvent*) override;

private:
	QWidget* left = nullptr;
	QWidget* right = nullptr;
	float left_s = 0.0f;
	float right_s = 0.0f;
	float total_s = 0.0f;
	float spacing = 0;
};

