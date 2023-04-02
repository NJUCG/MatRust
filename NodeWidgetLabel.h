#pragma once
#include"NodeData.h"
#include"CssLoader.h"
#include<qlabel.h>
#include"UIModel.h"

class NodeWidgetLabel:public QLabel, public NodeDataResponder
{
public:
	NodeWidgetLabel();
	void on_streamed(void*,int) override;
	void value_changed(void*,int) override;
	void on_cutoff(void*,int) override;
	void set_node_data(NodeData<QString>*);
	QString default_msg = "";
protected:
	bool streamed = false;
	NodeData<QString>* node_data = nullptr;
	void init();
	void addComponent();
};

