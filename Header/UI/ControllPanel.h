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
#include"LayerTab.h"
#include"BakeTab.h"
#include"qlayout.h"
#include"OutputTab.h"
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
	unordered_map<string, ControllTabWidgetResponder*> responders_dict;
	UIModel* model;
	QWidget* container = nullptr;
	QWidget* current_tab = nullptr;
	ControllTabWidgetResponder* current_responder = nullptr;
	QVBoxLayout* layout;
	void init();
	void addComponent();
};

