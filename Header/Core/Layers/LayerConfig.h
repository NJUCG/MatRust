#pragma once
#include<glm/glm.hpp>
#include<string>
#include<unordered_map>
#include<qstring.h>
using namespace glm;
using namespace std;
class LayerConfig {
public:
	QString layer_name = "";
	string layer_type = "perlin";
	string alg_type = "";

	vec4 color;
	float ac;
	float roughness;
	float metallic;

	unordered_map<string, void*> addition_properties;
};
