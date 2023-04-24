#include "CanvasBackGround.h"

CanvasBackGround::CanvasBackGround()
{
	init();
}

void CanvasBackGround::Draw()
{
	QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
	f->glBindVertexArray(VAO);
	//f->glDrawElements(GL_LINES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
	f->glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);

	f->glBindVertexArray(0);
}

void CanvasBackGround::init()
{
	vec3 tv[4] = {
		vec3(1,1,0),
		vec3(1,-1,0),
		vec3(-1,-1,0),
		vec3(-1,1,0)
	};
	unsigned int ti[8]
		= {
		0,1,2,
		0,2,3,
	};
	vertices = vector<Vertex>();
	indices = vector<unsigned int>();
	
	for (int i = 0; i < 4; i++) {
		Vertex v;
		v.Position = tv[i];
		vertices.push_back(v);
	}
	for (int i = 0; i < 8; i++) {
		indices.push_back(ti[i]);
	}
	
	QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
	f->glGenVertexArrays(1, &VAO);
	f->glGenBuffers(1, &VBO);
	f->glGenBuffers(1, &EBO);
	
	f->glBindVertexArray(VAO);

	f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
	f->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	f->glEnableVertexAttribArray(0);
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	f->glBindVertexArray(0);
}
