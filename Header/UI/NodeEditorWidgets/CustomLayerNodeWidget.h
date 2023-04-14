#pragma once
#include"NodeWidget.h"
#include"LabeledComboBox.h"
class CustomLayerNodeWidget: public NodeWidget, public LabeledComboBoxResponder
{
public:
	CustomLayerNodeWidget(NodeWidgetResponder*, int, int);
	void value_changed(QString, QString) override;
	~CustomLayerNodeWidget();
protected:
	void init();
	void addComponent();
	void body() override;
	void prepare_arguments() override;
};

