#pragma once
#include"NodeWidget.h"
#include"LabeledComboBox.h"
#include"NodeDataResponder.h"

class DPDLayerNodeWidget:public NodeWidget, public NodeDataResponder
{
public:
	DPDLayerNodeWidget(NodeWidgetResponder*, int, int);
	~DPDLayerNodeWidget();

	virtual void value_changed(void*, int) override;
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

