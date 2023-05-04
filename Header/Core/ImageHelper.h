#pragma once
#include <QtGui/qopenglextrafunctions.h>
#include<iostream>
#include<glm/glm.hpp>

using namespace std;
using namespace glm;
class ImageHelper
{
public:
	ImageHelper();
	static void save_pic(string loc, unsigned char* rgb, int comp = 3, int w = 256, int h = 256);
	static unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
	static void load_pic(string path, vector<vector<vec4>>& pic_data, int& comp);
};

