#pragma once
#include<qwidget.h>
#include"FloatEditLabel.h"
#include"FloatEditLineEdit.h"
#include<qlayout.h>
#include"CssLoader.h"
#include"FloatEditPushButton.h"
#include<qevent.h>
#include<string>
#include"FloatMiddleButton.h"
#include"UIModel.h"
#include<qstackedlayout.h>
#include<sstream>
#include<iomanip>
#include"StackedWidget.h"
#include<vector>
#include<future>
#include"FloatEditResponder.h"
using namespace std;
enum EditorState {
	IDLE,
	HOVER,
	EDIT
};
// 默认作为label
// hover时出现左右箭头并可以拖动
// 点击后变为line edit
class FloatEdit: public QWidget, public FloatEditDelegate
{
public:
	FloatEdit();
	void set_min_value(float);
	void set_max_value(float);
	inline void set_value(float new_value, bool respond = true);
	float value;
	float min_value, max_value;
	float step;
	float sensitivity;
	FloatEditResponder* responder = nullptr;
	string name = "default";
	string unit = "%";
protected:
	QWidget* current_page = nullptr;
	vector<QWidget*> pages;
	EditorState current_state;
	FloatEditMiddleButton* middle_button = nullptr;
	FloatEditPushButton* left_button = nullptr;
	FloatEditPushButton* right_button = nullptr;
	FloatEditLineEdit* editor = nullptr;
	FloatEditLabel* value_label = nullptr;
	QWidget* hover_mode = nullptr;
	void init();
	void addComponent();
	void switchTo(EditorState state);
	void enterEvent(QEnterEvent*) override;
	void leaveEvent(QEvent*) override;
	void mouseMoveEvent(QMouseEvent*) override;
	// void resizeEvent(QResizeEvent*) override;

	void middle_button_mouse_move(QMouseEvent*) override;
	void middle_button_mouse_enter(QEnterEvent*) override;
    void middle_button_mouse_release(QMouseEvent*) override;
	void middle_button_mouse_leave(QEvent*) override;
	void middle_button_mouse_press(QMouseEvent*) override;
	void label_mouse_enter(QEnterEvent*) override;
	void pushbutton_mouse_leave(QEvent*) override;
	void pushbutton_mouse_enter(QEnterEvent*) override;
	void edit_mouse_leave(QEvent*) override;
	void edit_finish_edit() override;
	string get_value_without_unit();
	float cursor_x = 0;

	bool draging = false;
	bool from_middle = false;
	bool from_button = false;

	string get_value();

	void setCurrentIndex(int);

	void resizeEvent(QResizeEvent*) override;

	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;

};

