#pragma once
#include"qwidget.h"
#include"qevent.h"
#include"SceneTabEntryResponder.h"
#include"UIModel.h"
#include"qlayout.h"
#include"qpushbutton.h"
#include"qicon.h"
#include"qlabel.h"

class SceneTabEntry: public QWidget
{
public:
	SceneTabEntry(bool is_odd, int offset, string type, string name, SceneTabEntryResponder* responder = nullptr);
	void set_icon(string);
	void set_name(string);
	void clear_focus();
	string type, name;
protected:
	void init();
	void addComponent();
	
	bool mouse_in = false;
	void enterEvent(QEnterEvent*) override;
	void leaveEvent(QEvent*) override;
	void mousePressEvent(QMouseEvent*) override;

	bool selected = false;
	bool is_odd;
	int offset_in_pixel;
	string default_style;

	QWidget* body;
	QWidget* icon_container;
	QVBoxLayout* icon_layout;

	QWidget* label_container;
	QVBoxLayout* label_layout;
	QLabel* label;

	QHBoxLayout* body_layout;

	SceneTabEntryResponder* responder;
};

