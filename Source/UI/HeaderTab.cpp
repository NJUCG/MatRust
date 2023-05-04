#include "HeaderTab.h"

HeaderTab::HeaderTab()
{
	init();
	addComponent();
}

HeaderTab::HeaderTab(QString tab_name)
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

	QVBoxLayout* tab_layout = new QVBoxLayout();
	tab_layout->setContentsMargins(2, 2, 2, 2);
	tab_layout->setSpacing(0);

	QLabel* tab = new QLabel();

	tab->setAlignment(Qt::AlignCenter);
	tab->setText(tab_name);

	tab_layout->addWidget(tab);

	setLayout(tab_layout);
}

void HeaderTab::expand()
{

}

void HeaderTab::shrink()
{

}

void HeaderTab::enterEvent(QEnterEvent* e)
{
	if (state == HEADER_TAB_IDLE) {
		setStyleSheet(hover_style.c_str());
		
		state = HEADER_TAB_HOVER;
	}
}

void HeaderTab::leaveEvent(QEvent* e)
{
	if (state != HEADER_TAB_IDLE) {
		setStyleSheet(default_style.c_str());

		if (state == HEADER_TAB_SELECTED) {
			shrink();
		}

		state = HEADER_TAB_IDLE;
	}
}

void HeaderTab::mousePressEvent(QMouseEvent*)
{
	/*
	if (state != HEADER_TAB_SELECTED) {
		expand();
		state = HEADER_TAB_SELECTED;
	}
	*/
	if (tab_name == HEADER_LOAD_ARCHIVE) {
		Archive::load_config(QFileDialog::getExistingDirectory(nullptr, "archive", "."));
	}
	else if (tab_name == HEADER_SAVE_ARCHIVE) {
		Archive::save_config(QFileDialog::getExistingDirectory(nullptr, "archive", "."));
	}
	else if (tab_name == HEADER_SAVE_OUTPUT) {
		QString path = QFileDialog::getExistingDirectory(nullptr, "archive", ".");
		EventAdapter::shared->push_data(&path);
		EventAdapter::shared->trigger_event("save_output");
	}
}
