#pragma once
#include"Mesh.h"
using namespace glm;

class CanvasBackGround
{
public:
	CanvasBackGround();
	void Draw();
protected:
	void init();
	unsigned int VAO, VBO, EBO;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
};

