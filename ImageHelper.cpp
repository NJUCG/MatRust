#include "ImageHelper.h"
#include"stb_header.h"

ImageHelper::ImageHelper()
{
}

void ImageHelper::save_pic(string loc, unsigned char* rgb, int comp, int w, int h)
{
	stbi_write_png(loc.c_str(), w, h, comp, rgb, 0);
}

unsigned int ImageHelper::TextureFromFile(const char* path, const string& directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
    unsigned int textureID;
    f->glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        f->glBindTexture(GL_TEXTURE_2D, textureID);
        f->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        f->glGenerateMipmap(GL_TEXTURE_2D);

        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void ImageHelper::load_pic(string filename, vector<vector<vec4>>& pic_data, int& nrComponents)
{
    int width, height;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        pic_data = vector<vector<vec4>>(height, vector<vec4>(width, vec4()));
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int cur_idx = (i * width + j) * nrComponents;
                vec4 unit;

                unit.r = data[cur_idx];
                unit.g = data[cur_idx + 1];
                unit.b = data[cur_idx + 2];
                if (nrComponents > 3) {
                    unit.a = data[cur_idx + 3];
                }

                pic_data[i][j] = unit;
            }
        }
    }
    stbi_image_free(data);
}

