#include "HeaderTab.h"

HeaderTab::HeaderTab()
{
	init();
	addComponent();
}

HeaderTab::HeaderTab(string tab_name)
{
	this->tab_name = tab_name;
	init();
	addComponent();
}

void HeaderTab::add_entry(string entry_tag, string entry_label, string entry_event)
{
	
}

void HeaderTab::init()
{
	default_style = CssLoader::load_css_str("header_tab_style.css");
	hover_style = CssLoader::load_css_str("header_tab_style_hover.css");
	
	setStyleSheet(default_style.c_str());
}

void HeaderTab::addComponent()
{
	UIModel* model = UIModel::get();
	int width = model->header_tab_width;

	setFixedWidth(width);
}

void HeaderTab::expand()
{
}

void HeaderTab::shrink()
{
}

void HeaderTab::enterEvent(QEnterEvent* e)
{
	if (!is_mouse_in) {
		is_mouse_in = true;
		setStyleSheet(hover_style.c_str());
	}
}

void HeaderTab::leaveEvent(QEvent* e)
{
	if (is_mouse_in) {
		is_mouse_in = false;
		setStyleSheet(default_style.c_str());
	}
}
