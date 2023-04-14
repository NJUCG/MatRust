#pragma once
#include<glm/glm.hpp>
#include<unordered_map>
#include<vector>
#include"MeshProperty.h"
#include"MyRandom.h"
#include<unordered_set>
#include<qdebug.h>

using namespace glm;
using namespace std;

class _3DSample
{
public:
	_3DSample();
	~_3DSample();
	Vertex position;
private:
};

class _3DTriangle
{
public:
	_3DTriangle(Vertex,Vertex,Vertex);
	~_3DTriangle();
	vec3 get_sample();
	vec2 get_uv();
	ivec3 cell_pos;
	vec2 uv;
private:
	vec3 a, b, c, s;
	double e1, e2;
};

class _3DCell {
public:
	ivec3 cell_pos;
	vector<Vertex> candidates;
	int s_idx = -1;
};

class _3DBlueNoiseSampler
{
public:
	virtual vector<_3DSample*> sample(vector<Vertex>,vector<unsigned int>,float) = 0;
	virtual vector<vec2> get_texture() = 0;
	static void init_samplers();
protected:

};

class PoissonSampler: public _3DBlueNoiseSampler
{
public:
	vector<_3DSample*> sample(vector<Vertex>, vector<unsigned int>,float) override;
	vector<vec2> get_texture() override;
	static PoissonSampler* shared;
	static void init_poisson_sampler();
protected:
	vector<Vertex> local_vertices;
	vector<_3DSample*> res;
	int x_s, y_s, z_s;
	void get_triangles();
	bool cmp_tri(_3DTriangle*, _3DTriangle*);
};
