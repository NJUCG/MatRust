#include "TabEntry.h"


TabEntry::TabEntry()
{
	init();
	addComponent();
}

void TabEntry::addEntry(TabEntry* entry)
{

}

void TabEntry::addComponent(QWidget* component)
{
	QWidget* container = new QWidget();
	QHBoxLayout* layout = new QHBoxLayout();

	layout->addStretch(3);
	layout->addWidget(component, 7);
	
	container->setLayout(layout);
	entry_layout->addWidget(container);
}

void TabEntry::init()
{
	setStyleSheet(CssLoader::load_css("tabentry.css"));
}

void TabEntry::addComponent()
{
	entry_layout = new QVBoxLayout();
	QWidget* container = new QWidget();
	QHBoxLayout* box_layout = new QHBoxLayout();
	QLabel* value_label = new QLabel("hello:");
	value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	value_label->setFixedWidth(100);
	box_layout->addWidget(value_label,3);
	FloatEdit* editor = new FloatEdit();
	box_layout->addWidget(editor,7);
	container->setLayout(box_layout);

	addComponent(container);
	setLayout(entry_layout);
}

void TabEntry::expand()
{
}

void TabEntry::shrink()
{
}
