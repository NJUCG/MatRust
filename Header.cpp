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
}
