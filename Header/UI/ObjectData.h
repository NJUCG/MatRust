#pragma once
#include<glm/glm.hpp>
#include<string>
using namespace glm;
using namespace std;
class ObjectData
{
public:
	// ���ڲ����������
	int idx;
	string name;
	string tag;
	string type;
	string path;
	bool has_path = false;
	vec4 loc;
	vec4 rot;
	vec4 scl;
};

