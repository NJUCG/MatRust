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
	LabeledComboBox(LabeledComboBoxResponder* ,string);
	void add_item(string);
	void on_streamed(void*) override;
	void value_changed(void*) override;
	void on_cutoff(void*) override;
	void set_node_data(NodeData<string>*);

protected:
	bool streamed = false;

	NodeData<string>* node_data = nullptr;
	LabeledComboBoxResponder* responder = nullptr;
	QHBoxLayout* top_layout = nullptr;
	QComboBox* combo_box = nullptr;
	QLabel* tag_label = nullptr;
	string tag;

	vector<string> items;

	void init();
	void addComponent();
};

