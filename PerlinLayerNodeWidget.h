#pragma once
#include"NodeWidget.h"
#include"LabeledComboBox.h"
#include"NodeDataResponder.h"

class PerlinLayerNodeWidget: public NodeWidget, public NodeDataResponder
{
public:
	PerlinLayerNodeWidget(NodeWidgetResponder*,int,int);
	~PerlinLayerNodeWidget();

	virtual void value_changed(void*,int) override;
	void read(const QJsonObject&) override;
	void write(QJsonObject&) override;
protected:
	void init();
	void addComponent();
	void body() override;
	void prepare_arguments() override;

	LayerConfig* config_cache = nullptr;
	int config_id = -1;
};

