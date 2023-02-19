#pragma once
#include"qwidget.h"
#include"HeaderTabEntry.h"
#include<unordered_map>
#include"qevent.h"
#include"CssLoader.h"

class HeaderTab : public QWidget
{
public:
	HeaderTab();
	HeaderTab(string);
	void add_entry(string, string,string);
protected:
	string tab_name;
	string default_style, hover_style;
	unordered_map<string, HeaderTabEntry*> entries;

	bool is_mouse_in = false;

	void init();
	void addComponent();
	void expand();
	void shrink();
	void enterEvent(QEnterEvent*) override;
	void leaveEvent(QEvent*) override;
};

