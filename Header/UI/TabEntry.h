#pragma once
#include"qwidget.h"
#include"CssLoader.h"
#include<qboxlayout.h>
#include<qlabel.h>
#include <QTextEdit>
#include"FloatEdit.h"
class TabEntry: public QWidget
{
public:
	TabEntry();
	void addEntry(TabEntry* entry);
	void addComponent(QWidget* component);
protected:
	QVBoxLayout* entry_layout;
	virtual void init();
	virtual void addComponent();
	virtual void expand();
	virtual void shrink();
};

