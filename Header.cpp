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

	add_tab(new HeaderTab("Model"));

	top_layout->addWidget(new QWidget());
	setLayout(top_layout);
}

void Header::add_tab(HeaderTab* tab)
{
	top_layout->addWidget(tab);
}
