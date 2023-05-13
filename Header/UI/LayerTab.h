#pragma once
#include"NodeEditor.h"
#include"ControllTabWidgetResponder.h"

class LayerTab: public QWidget, public ControllTabWidgetResponder
{
public:
	LayerTab();
	~LayerTab();
protected:
	QVBoxLayout* top_layout = nullptr;
	NodeEditor* editor = nullptr;

	void init();
	void addComponent();

	void header();
};

