#include "Header.h"

Header::Header()
{
	init();
	addComponent();
}

void Header::init()
{
	setStyleSheet(CssLoader::load_css("header.css"));
}

void Header::addComponent()
{
	top_layout = new QHBoxLayout();
	top_layout->setContentsMargins(2, 2, 2, 2);
	top_layout->setSpacing(5);

	QPushButton* icon = new QPushButton();
	icon->setIcon(QIcon("resources/ui/icons/icon-30.png"));
	icon->setFixedSize(20, 20);
	icon->setEnabled(false);
	icon->setFlat(true);
	top_layout->addWidget(icon);

	add_tab(new HeaderTab(HEADER_LOAD_ARCHIVE));
	add_tab(new HeaderTab(HEADER_SAVE_ARCHIVE));
	add_tab(new HeaderTab(HEADER_SAVE_OUTPUT));

	top_layout->addWidget(new QWidget());
	setLayout(top_layout);
}

void Header::add_tab(HeaderTab* tab)
{
	top_layout->addWidget(tab);
}
