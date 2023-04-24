#include "SimpleEntry.h"

SimpleEntry::SimpleEntry():TabEntry()
{
	init();
	addComponent();
}

void SimpleEntry::init()
{
	TabEntry::init();
}

void SimpleEntry::addComponent()
{
	tree = new QTreeView(this);

}

