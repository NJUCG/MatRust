#include "CustomLayer.h"

CustomLayer::CustomLayer()
{
}

CustomLayer::CustomLayer(string rule, string alg_type)
{
	this->rule = rule;
	this->alg_type = alg_type;
}

CustomLayer::CustomLayer(LayerConfig* config)
{
	set_addition_config(config);
}

void CustomLayer::accept_rules(PipelineConfig* config)
{
	Layer::accept_rules(config);
}

void CustomLayer::set_addition_config(LayerConfig* addition_config)
{
	alg_type = addition_config->alg_type;
}

void CustomLayer::rust(double)
{

}
