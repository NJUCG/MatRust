#include "HeaderTabEntry.h"

HeaderTabEntry::HeaderTabEntry()
{
	init();
	addComponent();
}

HeaderTabEntry::HeaderTabEntry(string e)
{
	init();
	addComponent();
	set_event(e);
}

void HeaderTabEntry::set_event(string e)
{
}

void HeaderTabEntry::init()
{
}

void HeaderTabEntry::addComponent()
{
}
