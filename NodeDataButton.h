#pragma once
#include<qwidget.h>
#include<qpushbutton.h>
#include"NodePool.h"
#include"CssLoader.h"
#include"UIModel.h"
#include"NodeDataButtonResponder.h"
#include<qevent.h>
class NodeDataButton:public QPushButton 
{
public:
	NodeDataButton(NodeDataButtonResponder*, NodePool*, int,bool,string color="orange");
	int id;
	bool is_adapter;
	QPoint center();
	int button_index = -1;
	string value_type;
protected:
	NodeDataButtonResponder* responder;
	NodePool* pool;
	void init();
	void addComponent();
	void on_click();

	bool is_linking = false;
	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void mouseMoveEvent(QMouseEvent*) override;

};

