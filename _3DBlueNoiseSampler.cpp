#include "_3DBlueNoiseSampler.h"

_3DSample::_3DSample()
{
}

_3DSample::~_3DSample()
{
}

_3DSampleIndex::_3DSampleIndex()
{
}

_3DSampleIndex::~_3DSampleIndex()
{
}

vector<_3DSample*> PoissonSampler::sample(vector<Vertex> vertices, vector<unsigned int> indices, float r)
{
	local_vertices = vertices;

	vector<_3DTriangle*> ts;

	unsigned int il = indices.size();
	unsigned int i = 0;

	while (i < il - 2) {
		Vertex a = vertices[indices[i]];
		Vertex b = vertices[indices[i + 1]];
		Vertex c = vertices[indices[i + 2]];
		i += 3;
		ts.push_back(new _3DTriangle(a, b, c));
	}

	vec3 lb = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 tr = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	// 搭好格子
	for (int i = 0; i < ts.size(); i++) {
		vec3 ct = ts[i]->get_sample();
		lb.x = ct.x < lb.x ? ct.x : lb.x;
		lb.y = ct.y < lb.y ? ct.y : lb.y;
		lb.z = ct.z < lb.z ? ct.z : lb.z;

		tr.x = tr.x < ct.x ? ct.x : tr.x;
		tr.y = tr.y < ct.y ? ct.y : tr.y;
		tr.z = tr.z < ct.z ? ct.z : tr.z;
	}
	
	float grid_len = r / sqrt(3.0);
	
	int lb_x_i = floor(lb.x / grid_len);
	int lb_y_i = floor(lb.y / grid_len);
	int lb_z_i = floor(lb.z / grid_len);

	int tr_x_i = ceil(tr.x / grid_len);
	int tr_y_i = ceil(tr.y / grid_len);
	int tr_z_i = ceil(tr.z / grid_len);

	x_s = tr_x_i - lb_x_i;
	y_s = tr_y_i - lb_y_i;
	z_s = tr_z_i - lb_z_i;

	unordered_map<_3Divec3, _3DCell*, hash_name> cell_hash;

	// 放入格子
	for (int i = 0; i < ts.size(); i++) {
		vec3 ct = ts[i]->get_sample();
		int x_i = floor(ct.x / grid_len);
		int y_i = floor(ct.y / grid_len);
		int z_i = floor(ct.z / grid_len);
		_3Divec3 idx(x_i, y_i, z_i);
		ts[i]->cell_pos = vec3(idx.x,idx.y,idx.z);
		if (cell_hash.find(idx) == cell_hash.end()) {
			cell_hash[idx] = new _3DCell();
		}
		Vertex v;
		v.Position = ct;
		v.TexCoords = ts[i]->get_uv();
		cell_hash[idx]->candidates.push_back(v);
	}

	// 排序
	int len = ts.size();
	for (int i = 0; i < len - 1; i++)
		for (int j = 0; j < len - 1 - i; j++)
			if (cmp_tri(ts[j], ts[j + 1])) {
				_3DTriangle* temp = ts[j];
				ts[j] = ts[j + 1];
				ts[j + 1] = temp;
			}
	
	for (pair<_3Divec3, _3DCell*> cell : cell_hash) {
		for (int ic = 0; ic < cell.second->candidates.size();ic++) {
			vec3 s = cell.second->candidates[ic].Position;
			bool conflict = false;
			for (int i = -1; i <= 1; i++) {
				if (conflict) {
					break;
				}
				for (int j = -1; j <= 1; j++) {
					if (conflict) {
						break;
					}
					for (int k = -1; k <= 1; k++) {
						if (!i && !j && !k) {
							continue;
						}
						_3Divec3 c(s.x + i, s.y + j, s.z + k);
						if (cell_hash.count(c) > 0) {
							if (cell_hash[c]->s_idx >= 0) {
								if ((cell_hash[c]->candidates[cell_hash[c]->s_idx].Position - s).length() < r) {
									conflict = true;
									break;
								}
							}
						}
					}
				}
			}
			if (!conflict) {
				cell.second->s_idx = ic;
				_3DSample* sample = new _3DSample();
				sample->position = cell.second->candidates[ic];
				res.push_back(sample);
				break;
			}
		}
	}

	for (pair<_3Divec3, _3DCell*> cell : cell_hash) {
		delete cell.second;
	}

	return res;
}

vector<vec2> PoissonSampler::get_texture()
{
	vector<vec2> textures;
	for (int i = 0; i < res.size(); i++) {
		textures.push_back(res[i]->position.TexCoords);
	}
	return textures;
}
PoissonSampler* PoissonSampler::shared = nullptr;
void PoissonSampler::init_poisson_sampler()
{
	shared = new PoissonSampler();
}

void PoissonSampler::get_triangles()
{

}

bool PoissonSampler::cmp_tri(_3DTriangle* a, _3DTriangle* b)
{
	return (a->cell_pos.x * y_s * z_s + a->cell_pos.y * z_s + a->cell_pos.z) 
		< (b->cell_pos.x * y_s * z_s + b->cell_pos.y * z_s + b->cell_pos.z);
}

_3DTriangle::_3DTriangle(Vertex a, Vertex b, Vertex c)
{
	this->a = a.Position;
	this->b = b.Position;
	this->c = c.Position;

	float e1 = MyRandom::_0_1();
	float e2 = MyRandom::_0_1();

	this->s = e1 * this->a + e2 * this->b;
	this->uv = e1 * a.TexCoords + e2 * b.TexCoords;
}

_3DTriangle::~_3DTriangle()
{

}

vec3 _3DTriangle::get_sample()
{
	return s;
}

vec2 _3DTriangle::get_uv()
{
	return uv;
}

void _3DBlueNoiseSampler::init_samplers()
{
	PoissonSampler::init_poisson_sampler();
}
