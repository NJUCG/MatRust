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

class _3DSampleIndex
{
public:
	_3DSampleIndex();
	~_3DSampleIndex();
	vec3 idx;
private:

};

class _3Divec3{
public:
	_3Divec3(int x, int y, int z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
	_3Divec3(){
	}
	bool operator==(const _3Divec3& other) const{
		return this->x == other.x && this->y == other.y && this->z == other.z;
	}

	int x, y, z;
};
struct hash_name {
	size_t operator()(const _3Divec3& self) const {
		static hash<std::string> hash_str;
		return hash_str(std::to_string(self.x) + " " + std::to_string(self.y) + " " + std::to_string(self.z));
	}
};

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
