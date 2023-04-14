#pragma once
#include <QString>
#include<string>
#include<iostream>
#include<fstream>
using namespace std;
class CssLoader
{
public:
	static QString load_css(string path);
	static string load_css_str(string path);
};

