#pragma once
#include"NodeWidget.h"

class LayerMachineNodeWidget: public NodeWidget
{
public:
	LayerMachineNodeWidget(NodeWidgetResponder*, int, int);
protected:
	void init();
	void addComponent();

	void prepare_arguments() override;
	void body() override;

	void add_layer_slot();
	void remove_layer_slot();
};

