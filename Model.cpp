#include"Model.h"

unsigned int Model::TextureFromFile(const char* path, const string& directory, bool gamma)
{
	return ImageHelper::TextureFromFile(path, directory, gamma);
}