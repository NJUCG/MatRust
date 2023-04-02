#pragma once
#include<glm/glm.hpp>
#include<string>
using namespace glm;
using namespace std;
class ObjectData
{
public:
	// 仅在部分情况有用
	int idx;
	string name;
	string tag;
	string type;
	string path;
	vec4 loc;
	vec4 rot;
	vec4 scl;
};

