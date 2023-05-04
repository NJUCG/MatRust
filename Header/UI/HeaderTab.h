#pragma once
#include"qwidget.h"
#include"HeaderTabEntry.h"
#include<unordered_map>
#include"qevent.h"
#include"CssLoader.h"
#include<qfiledialog.h>
#include"EventAdapter.h"
#include"Archive.h"
#include"StringHelper.h"
#include"OutputHelper.h"

#define HEADER_TAB_IDLE 0
#define HEADER_TAB_HOVER 1
#define HEADER_TAB_SELECTED 2


class HeaderTab : public QWidget
{
public:
	HeaderTab();
	HeaderTab(QString);
	void add_entry(string, string,string);
protected:
	QString tab_name;
	string default_style, hover_style;
	unordered_map<string, HeaderTabEntry*> entries;

	bool is_mouse_in = false;
	int state;

	void init();
	void addComponent();
	void expand();
	void shrink();
	void enterEvent(QEnterEvent*) override;
	void leaveEvent(QEvent*) override;
	void mousePressEvent(QMouseEvent*) override;
};

