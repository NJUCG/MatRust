#pragma once
#include<string>
using namespace std;

class FileSelectorResponder
{
public:
	virtual void on_select_file(string,string);
};

