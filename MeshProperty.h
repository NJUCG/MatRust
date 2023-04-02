#pragma once
#include<glm/glm.hpp>
#include<string>
#define MAX_BONE_INFLUENCE 4
using namespace std;

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};
class _3DSampleIndex
{
public:
	_3DSampleIndex();
	~_3DSampleIndex();
	glm::vec3 idx;
private:

};

class _3Divec3 {
public:
	_3Divec3(int x, int y, int z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	_3Divec3() {
	}
	bool operator==(const _3Divec3& other) const {
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

class _3Dvec3 {
public:
	_3Dvec3(int x, int y, int z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	_3Dvec3(glm::vec3 v) {
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	}
	_3Dvec3() {
	}
	bool operator==(const _3Dvec3& other) const {
		return this->x == other.x && this->y == other.y && this->z == other.z;
	}
	int x, y, z;
};

struct hash_vec3 {
	size_t operator()(const _3Dvec3& self) const {
		static hash<std::string> hash_str;
		return hash_str(std::to_string(self.x) + " " + std::to_string(self.y) + " " + std::to_string(self.z));
	}
};