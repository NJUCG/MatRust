#include "DPDLayerNodeWidget.h"

DPDLayerNodeWidget::DPDLayerNodeWidget(NodeWidgetResponder* responder, int loc_x, int loc_y) : NodeWidget(responder, loc_x, loc_y, false)
{
	init();
	addComponent();
}

DPDLayerNodeWidget::~DPDLayerNodeWidget()
{
}

void DPDLayerNodeWidget::value_changed(void* v, int id)
{
	if (id == data_cache["transmittance"]) {
		config_cache->ac = *((float*)v);
	}
	else if (id == data_cache["roughness"]) {
		config_cache->roughness = *((float*)v);
	}
	else if (id == data_cache["metallic"]) {
		config_cache->metallic = *((float*)v);
	}
	else if (id == data_cache["part per sec"]) {
		config_cache->addition_properties["part per sec"] = v;
	}
	else if (id == data_cache["name"]) {
		config_cache->layer_name = *((QString*)v);
	}
	else if (id == data_cache["color"]) {
		QColor color = *((QColor*)v);
		config_cache->color = vec4(color.red(), color.green(), color.blue(), 255);
	}
	else if (id == data_cache["grow spd"]) {
		config_cache->addition_properties["grow spd"] = v;
	}
	else if (id == data_cache["init seed"]) {
		config_cache->addition_properties["init seed"] = v;
	}
	else if (id == data_cache["randomizer"]) {
		config_cache->addition_properties["randomizer"] = v;
	}
	else if (id == data_cache["diffuse spd"]) {
		config_cache->addition_properties["diffuse spd"] = v;
	}
	pool->set_value<LayerConfig*>(config_id, config_cache);
}

void DPDLayerNodeWidget::read(const QJsonObject& obj)
{
	pool->set_value<float>(data_cache["transmittance"], obj["transmittance"].toDouble());
	pool->set_value<float>(data_cache["roughness"], obj["roughness"].toDouble());
	pool->set_value<float>(data_cache["metallic"], obj["metallic"].toDouble());
	pool->set_value<QString>(data_cache["part per sec"], obj["part per sec"].toString());
	pool->set_value<QString>(data_cache["grow spd"], obj["grow spd"].toString());
	pool->set_value<float>(data_cache["init seed"], obj["init seed"].toDouble());
	pool->set_value<float>(data_cache["randomizer"], obj["randomizer"].toDouble());
	pool->set_value<float>(data_cache["diffuse spd"], obj["diffuse spd"].toDouble());
	pool->set_value<QString>(data_cache["name"], obj["name"].toString());

	QColor color;
	color.setRed(obj["color_r"].toInt());
	color.setGreen(obj["color_g"].toInt());
	color.setBlue(obj["color_b"].toInt());
	pool->set_value<QColor>(data_cache["color"], color);

	NodeWidget::read(obj);
}

void DPDLayerNodeWidget::write(QJsonObject& json)
{
	json["transmittance"] = pool->get_value<float>(data_cache["transmittance"]);
	json["roughness"] = pool->get_value<float>(data_cache["roughness"]);
	json["metallic"] = pool->get_value<float>(data_cache["metallic"]);
	json["randomizer"] = pool->get_value<float>(data_cache["randomizer"]);
	int t0 = data_cache["part per sec"];
	json["part per sec"] = pool->get_value<QString>(data_cache["part per sec"]);
	json["grow spd"] = pool->get_value<QString>(data_cache["grow spd"]);
	json["init seed"] = pool->get_value<float>(data_cache["init seed"]);
	json["diffuse spd"] = pool->get_value<float>(data_cache["diffuse spd"]);

	json["name"] = pool->get_value<QString>(data_cache["name"]);
	QColor color = pool->get_value<QColor>(data_cache["color"]);
	json["color_r"] = color.red();
	json["color_g"] = color.green();
	json["color_b"] = color.blue();

	json["type"] = "DPD";

	NodeWidget::write(json);
}

void DPDLayerNodeWidget::init()
{
	NodeWidget::init();
	prepare_arguments();
}

void DPDLayerNodeWidget::addComponent()
{
	wrapper_layout = new QVBoxLayout();
	wrapper_layout->setContentsMargins(10, 10, 10, 10);

	wrapper_widget = new QWidget();
	top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(0);
	header();
	body_widget = new QWidget();
	body_widget->setObjectName("body_widget");
	body_layout = new QVBoxLayout();
	body_layout->setSpacing(5);
	body_layout->setContentsMargins(5, 5, 5, 5);
	current_height = 15 + model->node_widget_header_height;
	DPDLayerNodeWidget::body();
	body_widget->setLayout(body_layout);
	top_layout->addWidget(body_widget);
	top_layout->addWidget(new QWidget());
	wrapper_widget->setLayout(top_layout);

	wrapper_layout->addWidget(wrapper_widget);
	setObjectName("outer");
	setLayout(wrapper_layout);
}

void DPDLayerNodeWidget::body()
{
	new_palette("color", true, false, true, true, NODE_EDITOR_NODE_WIDGET_ENTRY_COLOR);

	new_float_argument(NODE_EDITOR_NODE_WIDGET_ENTRY_TRANSMITTANCE, "transmittance",false, false, -100.0f, 0.0f);
	new_float_argument(NODE_EDITOR_NODE_WIDGET_ENTRY_ROUGHNESS, "roughness",false, false);
	new_float_argument(NODE_EDITOR_NODE_WIDGET_ENTRY_METALLIC, "metallic", false, false);
	new_float_argument(NODE_EDITOR_NODE_WIDGET_ENTRY_DPD_RANDOMIZER, "randomizer", false, false, 1, 20, 1.0f);
	new_float_argument(NODE_EDITOR_NODE_WIDGET_ENTRY_DIFFUSE_SPD, "diffuse spd", false, false, 1, 20, 1.0f);

	new_line_edit(" 200 / ( sc + 0.01 ) * ( oc + 0.01 ) * ( rh + 0.01 )","part per sec", false, false, true, NODE_EDITOR_NODE_WIDGET_ENTRY_PART_PER_SEC);
	new_line_edit("sc * 0.8", "grow spd", false, false, true, NODE_EDITOR_NODE_WIDGET_ENTRY_GROW_SPD);
	new_float_argument(NODE_EDITOR_NODE_WIDGET_ENTRY_INIT_SEED,"init seed", false, false, 0, 30, 1.0f);

	new_line_edit(NODE_EDITOR_NODE_WIDGET_ENTRY_DEFAULT_NAME,"name", false, false, true, NODE_EDITOR_NODE_WIDGET_ENTRY_NAME);

	config_cache = new LayerConfig();
	config_cache->layer_name = "output";
	config_cache->alg_type = "DPD";
	config_cache->addition_properties["part per sec"] = new QString(" 200 / ( sc + 0.01 ) * ( oc + 0.01 ) * ( rh + 0.01 )");
	config_cache->addition_properties["grow spd"] = new QString("sc * 0.8");
	config_cache->addition_properties["init seed"] = new float(4.0f);

	config_id = new_layer_config(config_cache, "config", data_cache["name"], true, true, NODE_EDITOR_NODE_WIDGET_ENTRY_CONFIG);

	pool->get_data<QString>(data_cache["part per sec"])->add_responder(this);
	pool->get_data<QString>(data_cache["grow spd"])->add_responder(this);
	pool->get_data<float>(data_cache["init seed"])->add_responder(this);
	pool->get_data<float>(data_cache["diffuse spd"])->add_responder(this);

	pool->get_data<float>(data_cache["transmittance"])->add_responder(this);
	pool->get_data<float>(data_cache["roughness"])->add_responder(this);
	pool->get_data<float>(data_cache["metallic"])->add_responder(this);
	pool->get_data<float>(data_cache["randomizer"])->add_responder(this);
	pool->get_data<string>(data_cache["name"])->add_responder(this);
	pool->get_data<QColor>(data_cache["color"])->add_responder(this);
}

void DPDLayerNodeWidget::prepare_arguments()
{
	title = NODE_EDITOR_NODE_WIDGET_DPD_LAYER_TITLE;
	title_color = string("background-color:#A349A4;");
}
