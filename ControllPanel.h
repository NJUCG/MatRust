#pragma once
#include <qwidget.h>
#include"CssLoader.h"
#include"ControllerSelector.h"
#include<unordered_map>
#include"ControllSelectorDelegate.h"
#include"ControlTabDelegate.h"
#include"EnvTab.h"
#include"ObjectTab.h"
#include"EngineTab.h"
#include"qlayout.h"
#include <QtWidgets/qstackedlayout.h>
using namespace std;
enum ControlTabType {
	Default,
	Bake,
	Env
};
class ControllPanel: public QWidget, public ControllSelectorDelegate, public ControlTabDelegate
{
public:
	ControllPanel();
	void switchTo(string type);
	void value_changed(ControlTabParam arg, float newValue) override;
protected:
	unordered_map<string, QWidget*> tabs_dict;
	UIModel* model;
	QWidget* container;
	QWidget* current_tab;
	QVBoxLayout* layout;
	void init();
	void addComponent();
};

