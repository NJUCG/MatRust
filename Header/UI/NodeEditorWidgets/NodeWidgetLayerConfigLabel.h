#pragma once
#include"NodeData.h"
#include"CssLoader.h"
#include<qlabel.h>
#include"LayerConfig.h"
#include"UIModel.h"

class NodeWidgetLayerConfigLabel:public QLabel, public NodeDataResponder
{
public:
	NodeWidgetLayerConfigLabel();
	void on_streamed(void*, int) override;
	void value_changed(void*, int) override;
	void on_cutoff(void*, int) override;
	void set_node_data(NodeData<LayerConfig*>*);
	QString default_msg = "";
protected:
	bool streamed = false;
	NodeData<LayerConfig*>* node_data = nullptr;
	void init();
	void addComponent();
};

