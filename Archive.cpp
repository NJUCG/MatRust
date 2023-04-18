#include "Archive.h"

Archive::Archive()
{
}

void Archive::save_config(QString path)
{
	QString env_config_path = path + "/" + "env.json";
	QString node_editor_config_path = path + "/" + "custom.json";
	QString canvas_config_path = path + "/" + "geometry.json";
	QString object_config_path = path + "/" + "obj.json";
	QString bake_config_path = path + "/" + "bake.json";

	EventAdapter::shared->push_data(&env_config_path);
	EventAdapter::shared->trigger_event("save_archive_event_env_tab");

	EventAdapter::shared->push_data(&node_editor_config_path);
	EventAdapter::shared->trigger_event("save_archive_event_node_editor");

	EventAdapter::shared->push_data(&object_config_path);
	EventAdapter::shared->trigger_event("save_archive_event_obj");

	EventAdapter::shared->push_data(&canvas_config_path);
	EventAdapter::shared->trigger_event("save_archive_event_canvas");

	EventAdapter::shared->push_data(&bake_config_path);
	EventAdapter::shared->trigger_event("save_archive_event_bake_tab");
}

void Archive::load_config(QString path)
{
	QString env_config_path = path + "/" + "env.json";
	QString node_editor_config_path = path + "/" + "custom.json";
	QString canvas_config_path = path + "/" + "geometry.json";
	QString object_config_path = path + "/" + "obj.json";
	QString bake_config_path = path + "/" + "bake.json";

	EventAdapter::shared->push_data(&env_config_path);
	EventAdapter::shared->trigger_event("load_archive_event_env_tab");

	EventAdapter::shared->push_data(&node_editor_config_path);
	EventAdapter::shared->trigger_event("load_archive_event_node_editor");

	EventAdapter::shared->push_data(&object_config_path);
	EventAdapter::shared->trigger_event("load_archive_event_obj");

	EventAdapter::shared->push_data(&canvas_config_path);
	EventAdapter::shared->trigger_event("load_archive_event_canvas");

	EventAdapter::shared->push_data(&bake_config_path);
	EventAdapter::shared->trigger_event("load_archive_event_bake_tab");
}
