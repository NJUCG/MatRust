#pragma once
#include"NodeWidget.h"
#include"NodeDataResponder.h"
#include"LayerConfig.h"

class LayerMachineNodeWidget: public NodeWidget, public NodeDataResponder
{
public:
	LayerMachineNodeWidget(NodeWidgetResponder*, int, int);
	void value_changed(void*, int) override;

	vector<LayerConfig*> get_layers();
	void read(const QJsonObject&) override;
	void write(QJsonObject&) override;
protected:
	void init();
	void addComponent();

	void prepare_arguments() override;
	void body() override;

	void add_layer_slot();
	void remove_layer_slot();

	void add_remove_btn();
};

