#pragma once
#include"NodeData.h"
#include"CssLoader.h"
#include<qlabel.h>
#include<qpushbutton.h>
#include<qlineedit.h>
#include"UIModel.h"
#include<sstream>
#include<string>
#include<qcolordialog.h>
using namespace std;

class NodeWidgetPalette :public QPushButton, public NodeDataResponder
{
public:
	NodeWidgetPalette();
	~NodeWidgetPalette();
	void on_streamed(void*,int) override;
	void value_changed(void*,int) override;
	void on_cutoff(void*,int) override;
	void set_node_data(NodeData<QColor>*);
	QColor current_color;
	void set_interactable(bool);
protected:
	bool streamed = false;
	bool interactable = true;
	NodeData<QColor>* node_data = nullptr;
	QColorDialog* dialog = nullptr;;
	void init();
	void addComponent();
	void update_color();
	string to_hex_string(int);
}; 

