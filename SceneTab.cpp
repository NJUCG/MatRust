#include "SceneTab.h"

SceneTab::SceneTab()
{
	init();
	addComponent();
}

void SceneTab::add_property(string type, string name)
{
	if (entries.find(name) == entries.end()) {
		SceneTabEntry* entry = new SceneTabEntry(entries.size() % 2 == 0, 1, type, name, this);
		int c = entries.size() + 1;
		top_layout->insertWidget(c, entry);
		entries[name] = entry;
	}
}


void SceneTab::remove_object(string name)
{
	if (entries.find(name) != entries.end()) {
		top_layout->removeWidget(entries[name]);
	}
}

void SceneTab::on_trigger(string name)
{
	if (name == "add_object_to_tab") {
		ObjectData* data = (ObjectData*)EventAdapter::shared->pop_data();
		if (objects.find(data->name) == objects.end()) {
			objects[data->name] = data;
			if (data->type == "mesh") {
				add_property("mesh-30", data->name);
			}
			else if (data->type == "light") {
				add_property("light-30", data->name);
			}
		}
	}
}

void SceneTab::init()
{
	setStyleSheet(CssLoader::load_css("scene.css"));
	entries = unordered_map<string, SceneTabEntry*>();
	objects = unordered_map<string, ObjectData*>();
	EventAdapter::shared->register_event("add_object_to_tab", this);
}

void SceneTab::addComponent()
{
	top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(0);
	header();
	top_layout->addWidget(new QWidget());
	setLayout(top_layout);
}

void SceneTab::header()
{
	QWidget* header_w = new QWidget();
	
	QVBoxLayout* header_layout = new QVBoxLayout();

	header_w->setStyleSheet("background-color:#2B2B2B;color:white;");

	header_layout->setContentsMargins(0, 0, 0, 0);
	header_layout->setSpacing(0);

	QLabel* header_label = new QLabel();
	header_label->setText(QString::fromLocal8Bit(string("ÎïÌå").c_str()));
	header_layout->addWidget(header_label);

	header_w->setLayout(header_layout);
	
	header_w->setFixedHeight(UIModel::get()->scene_entry_line_height);
	header_w->setMinimumHeight(UIModel::get()->scene_entry_line_height);
	setMinimumHeight(UIModel::get()->scene_entry_line_height);
	top_layout->addWidget(header_w);
}

void SceneTab::on_select(string name)
{
	for (unordered_map<string, SceneTabEntry*>::iterator i = entries.begin(); i != entries.end(); i++) {
		SceneTabEntry* entry = (*i).second;
		if (entry->name != name) {
			entry->clear_focus();
		}
	}
	EventAdapter::shared->push_data(objects[name]);
	EventAdapter::shared->trigger_event("selected_object_changed");
}
