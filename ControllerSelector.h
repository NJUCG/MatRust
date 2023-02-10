#pragma once
#include<qwidget.h>
#include<unordered_map>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "UIModel.h"
#include"CssLoader.h"
#include"ControllSelectorDelegate.h"
using namespace std;
class ControllerSelector: public QWidget
{
public:
	ControllerSelector(ControllSelectorDelegate* delegator);
protected:
	unordered_map<string, QPushButton*> selector_dict;
	UIModel* model;
	ControllSelectorDelegate* delegator;
private:
	void init();
	void addComponent();
	void addPanelButton(string type, QVBoxLayout* layout);
};

