#pragma once
#include<qwidget.h>
#include<qlayout.h>
#include<qlabel.h>
#include<qpushbutton.h>
#include<qevent.h>

class ScalableContainer:public QWidget
{
public:
	ScalableContainer(QWidget* content);

	void mouseMoveEvent(QMouseEvent*) override;
	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	
private:
	QWidget* content = nullptr;
	QLabel* pull_btn = nullptr;

	void init();
	void addComponent();

	bool stretching = false;
	float delta_y = 0.0f;
	QPoint prev_pos;
};

