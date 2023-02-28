#pragma once
#include"NodeData.h"
#include"CssLoader.h"
#include<qlabel.h>
#include"UIModel.h"

class NodeWidgetLabel:public QLabel, public NodeDataResponder
{
public:
	NodeWidgetLabel();
	void on_streamed(void*) override;
	void value_changed(void*) override;
	void on_cutoff(void*) override;
	void set_node_data(NodeData<string>*);
protected:
	bool streamed = false;
	NodeData<string>* node_data = nullptr;
	void init();
	void addComponent();
};

