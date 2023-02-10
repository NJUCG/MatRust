#include "CssLoader.h"

QString CssLoader::load_css(string path)
{
    ifstream is;
    is.open(path, ios::in);
    istreambuf_iterator<char> begin(is);
    istreambuf_iterator<char> end;
    string temp_string = string(begin, end);
    is.close();
    return QString(temp_string.c_str());
}
