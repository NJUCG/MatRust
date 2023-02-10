#pragma once
#include"qwidget.h"
#include"CssLoader.h"
#include"SceneTabEntry.h"
#include"EventAdapter.h"
#include"ObjectData.h"

class SceneTab: public QWidget, public SceneTabEntryResponder, public EventResponder
{
public:
	SceneTab();
	void add_property(string type, string name);
	void remove_object(string name);
	void on_trigger(string) override;
protected:
	unordered_map<string, SceneTabEntry*> entries;
	unordered_map<string, ObjectData*> objects;
	QVBoxLayout* top_layout = nullptr;
	void init();
	void addComponent();
	void header();
	void on_select(string) override;
};

