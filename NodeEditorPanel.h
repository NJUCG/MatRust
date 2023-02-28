#pragma once
#include<qwidget.h>
#include"NodeEditorPanelResponder.h"
#include<qpushbutton.h>
#include<qlayout.h>
#include<string>
#include"NodeEditorPanel.h"
#include"CssLoader.h"
using namespace std;

class NodeEditorPanel: public QWidget
{
public:
	NodeEditorPanel();
	~NodeEditorPanel();

	NodeEditorPanelResponder* responder = nullptr;

protected:
	QVBoxLayout* top_layout = nullptr;
	void init();
	void addComponent();

	void add_btn(string, string);
};

