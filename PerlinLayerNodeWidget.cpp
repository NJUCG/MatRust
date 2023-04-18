#include "PerlinLayerNodeWidget.h"

PerlinLayerNodeWidget::PerlinLayerNodeWidget(NodeWidgetResponder* responder, int loc_x, int loc_y) : NodeWidget(responder, loc_x, loc_y, false)
{
	init();
	addComponent();
}

PerlinLayerNodeWidget::~PerlinLayerNodeWidget()
{
}

void PerlinLayerNodeWidget::value_changed(void* v,int id)
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
	else if (id == data_cache["block width"]) {
		config_cache->addition_properties["block width"] = v;
	}
	else if (id == data_cache["name"]) {
		config_cache->layer_name = *((QString*)v);
	}
	else if (id == data_cache["color"]) {
		QColor color = *((QColor*)v);
		config_cache->color = vec4(color.red(), color.green(), color.blue(), 255);
	}
	else if (id == data_cache["bias height"]) {
		config_cache->addition_properties["bias height"] = v;
	}
	else if (id == data_cache["cutoff height"]) {
		config_cache->addition_properties["cutoff height"] = v;
	}
	else if (id == data_cache["diffuse spd"]) {
		config_cache->addition_properties["diffuse spd"] = v;
	}
	pool->set_value<LayerConfig*>(config_id, config_cache);
}

void PerlinLayerNodeWidget::read(const QJsonObject& obj)
{
	pool->set_value<float>(data_cache["transmittance"], obj["transmittance"].toDouble());
	pool->set_value<float>(data_cache["roughness"], obj["roughness"].toDouble());
	pool->set_value<float>(data_cache["metallic"], obj["metallic"].toDouble());
	pool->set_value<float>(data_cache["bias height"], obj["bias height"].toDouble());
	pool->set_value<float>(data_cache["cutoff height"], obj["cutoff height"].toDouble());
	pool->set_value<float>(data_cache["block width"], obj["block width"].toDouble());
	pool->set_value<float>(data_cache["diffuse spd"], obj["diffuse spd"].toDouble());

	pool->set_value<QString>(data_cache["name"], obj["name"].toString());

	QColor color;
	color.setRed(obj["color_r"].toInt());
	color.setGreen(obj["color_g"].toInt());
	color.setBlue(obj["color_b"].toInt());
	pool->set_value<QColor>(data_cache["color"], color);

	NodeWidget::read(obj);
}

void PerlinLayerNodeWidget::write(QJsonObject& json)
{
	json["transmittance"] = pool->get_value<float>(data_cache["transmittance"]);
	json["roughness"] = pool->get_value<float>(data_cache["roughness"]);
	json["metallic"] = pool->get_value<float>(data_cache["metallic"]);
	json["bias height"] = pool->get_value<float>(data_cache["bias height"]);
	json["cutoff height"] = pool->get_value<float>(data_cache["cutoff height"]);
	json["block width"] = pool->get_value<float>(data_cache["block width"]);
	json["diffuse spd"] = pool->get_value<float>(data_cache["diffuse spd"]);

	json["name"] = pool->get_value<QString>(data_cache["name"]);
	QColor color = pool->get_value<QColor>(data_cache["color"]);
	json["color_r"] = color.red();
	json["color_g"] = color.green();
	json["color_b"] = color.blue();
	json["type"] = "perlin";

	NodeWidget::write(json);
}

void PerlinLayerNodeWidget::init()
{
	NodeWidget::init();
	prepare_arguments();
}

void PerlinLayerNodeWidget::addComponent()
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
	PerlinLayerNodeWidget::body();
	body_widget->setLayout(body_layout);
	top_layout->addWidget(body_widget);
	top_layout->addWidget(new QWidget());
	wrapper_widget->setLayout(top_layout);

	wrapper_layout->addWidget(wrapper_widget);
	setObjectName("outer");
	setLayout(wrapper_layout);
}

void PerlinLayerNodeWidget::body()
{
	new_palette("color", true, false, true, true, NODE_EDITOR_NODE_WIDGET_ENTRY_COLOR);

	new_float_argument(NODE_EDITOR_NODE_WIDGET_ENTRY_TRANSMITTANCE,"transmittance", false, false, -100.0f, 0.0f);
	new_float_argument(NODE_EDITOR_NODE_WIDGET_ENTRY_ROUGHNESS, "roughness",false, false);
	new_float_argument(NODE_EDITOR_NODE_WIDGET_ENTRY_METALLIC, "metallic", false, false);

	new_float_argument(NODE_EDITOR_NODE_WIDGET_ENTRY_BIAS_HEIGHT, "bias height", false, false, 1, 100, 1.0f);
	new_float_argument(NODE_EDITOR_NODE_WIDGET_ENTRY_CUTOFF_HEIGHT, "cutoff height", false, false, 1, 100, 1.0f);
	new_float_argument(NODE_EDITOR_NODE_WIDGET_ENTRY_BLOCK_WIDTH, "block width", false, false, 1, 100, 1.0f);
	new_float_argument(NODE_EDITOR_NODE_WIDGET_ENTRY_DIFFUSE_SPD, "diffuse spd", false, false, 1, 20, 1.0f);


	new_line_edit(NODE_EDITOR_NODE_WIDGET_ENTRY_DEFAULT_NAME, "name", false, false, true, NODE_EDITOR_NODE_WIDGET_ENTRY_NAME);

	config_cache = new LayerConfig();
	config_cache->layer_name = "output";
	config_cache->alg_type = "perlin";
	config_cache->addition_properties["bias height"] = new float(0.0f);
	config_cache->addition_properties["cutoff height"] = new float(0.0f);
	config_cache->addition_properties["block width"] = new float(10.0f);

	config_id = new_layer_config(config_cache,"config", data_cache["name"], true, true, NODE_EDITOR_NODE_WIDGET_ENTRY_CONFIG);

	pool->get_data<float>(data_cache["transmittance"])->add_responder(this);
	pool->get_data<float>(data_cache["roughness"])->add_responder(this);
	pool->get_data<float>(data_cache["metallic"])->add_responder(this);
	pool->get_data<float>(data_cache["diffuse spd"])->add_responder(this);

	pool->get_data<float>(data_cache["bias height"])->add_responder(this);
	pool->get_data<float>(data_cache["cutoff height"])->add_responder(this);
	pool->get_data<float>(data_cache["block width"])->add_responder(this);
	pool->get_data<QString>(data_cache["name"])->add_responder(this);
	pool->get_data<QColor>(data_cache["color"])->add_responder(this);
}

void PerlinLayerNodeWidget::prepare_arguments()
{
	title = NODE_EDITOR_NODE_WIDGET_PERLIN_LAYER_TITLE;
	title_color = string("background-color:#2B652B;");
}
