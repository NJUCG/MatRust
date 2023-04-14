#pragma once
#include<qcombobox.h>
#include<qlabel.h>
#include<qlayout.h>
#include"CssLoader.h"
#include<vector>
#include"LabeledComboBoxResponder.h"
#include"NodeData.h"
#include"UIModel.h"
using namespace std;

class LabeledComboBox: public QWidget, public NodeDataResponder
{
public:
	LabeledComboBox(LabeledComboBoxResponder* ,QString);
	void add_item(QString);
	void on_streamed(void*,int) override;
	void value_changed(void*,int) override;
	void on_cutoff(void*,int) override;
	void set_node_data(NodeData<QString>*);

protected:
	bool streamed = false;

	NodeData<QString>* node_data = nullptr;
	LabeledComboBoxResponder* responder = nullptr;
	QHBoxLayout* top_layout = nullptr;
	QComboBox* combo_box = nullptr;
	QLabel* tag_label = nullptr;
	QString tag;

	vector<QString> items;

	void init();
	void addComponent();
};

