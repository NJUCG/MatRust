#pragma once
#include"TabEntry.h"
#include <QTreeView>
#include<qtreewidget.h>
#include<qtreewidgetitemiterator.h>
class SimpleEntry:public TabEntry
{
public:
	SimpleEntry();
protected:
	QTreeView* tree;
	void init() override;
	void addComponent() override;
};

