#pragma once
#include"ObjectData.h"
class LightData: public ObjectData
{
public:
	vec3 light_intensity;
	string light_name;
};

