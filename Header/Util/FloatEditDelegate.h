#pragma once
#include<qevent.h>
class FloatEditDelegate
{
public:
	virtual void middle_button_mouse_move(QMouseEvent*);
	virtual void middle_button_mouse_enter(QEnterEvent*);
	virtual void middle_button_mouse_release(QMouseEvent*);
	virtual void middle_button_mouse_leave(QEvent*);
	virtual void middle_button_mouse_press(QMouseEvent*);
	virtual void label_mouse_enter(QEnterEvent*);
	virtual void pushbutton_mouse_leave(QEvent*);
	virtual void pushbutton_mouse_enter(QEnterEvent*);
	virtual void edit_mouse_leave(QEvent*);
	virtual void edit_finish_edit();
	
};

