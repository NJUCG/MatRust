#pragma once
#include<qlabel.h>
#include<qpushbutton.h>
#include"Cssloader.h"
#include<qlayout.h>
#include<qpropertyanimation.h>
#include<qsizepolicy.h>
class ExpandableNode : public QWidget {
public:
	ExpandableNode();
	ExpandableNode(QString, QWidget*);
	void set_title(QString);
	void set_content(QWidget*);
	void expand();
	void shrink();

protected:
	void init();
	void addComponent();

	QVBoxLayout* top_layout = nullptr;

	QWidget* title_container = nullptr;
	QLabel* title = nullptr;
	QIcon* right_icon = nullptr;
	QIcon* down_icon = nullptr;

	QWidget* container = nullptr;
	QWidget* content = nullptr;
	QVBoxLayout* container_layout = nullptr;

	int container_height = 0;
	int title_height = 20;
	bool expanded = true;

	QPropertyAnimation* animator = nullptr;
};