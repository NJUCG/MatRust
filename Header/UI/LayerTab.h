#pragma once
#include<qwidget.h>
#include"NodeEditor.h"

class LayerTab: public QWidget
{
public:
	LayerTab();
	~LayerTab();
protected:
	QVBoxLayout* top_layout = nullptr;
	NodeEditor* editor = nullptr;

	void init();
	void addComponent();
};

