#pragma once
#include<qwidget.h>
#include<qlabel.h>
#include<qlayout.h>
#include<qfiledialog.h>
#include"CssLoader.h"
#include"UIModel.h"
#include"FileSelectorResponder.h"
#include<qevent.h>
#include"EventAdapter.h"
#include"string_defs.h"
#include"StringHelper.h"

class FileSelector: public QWidget
{
public:
	FileSelector();
	void set_name(QString);
	FileSelectorResponder* responder = nullptr;
	string event_name = "";
	string tag = "";
protected:
	QHBoxLayout* top_layout = nullptr;

	QLabel* name_tag = nullptr;

	QLabel* normal_label = nullptr;

	void init();
	void addComponent();

	void mousePressEvent(QMouseEvent*) override;

};

