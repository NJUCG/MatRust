#pragma once
#include<unordered_map>
#include<string>
#include"PipelineConfig.h"
#include<sstream>
#include<stack>
using namespace std;

typedef float(*alg_step)(PipelineConfig*);

class AlgArgAnalyzer
{
public:
	static AlgArgAnalyzer* analyzer;
	static void init_analyzer();
	float get_arg(string rules);
	void register_config(PipelineConfig*);
	void clear_config();

	void register_custom_config(unordered_map<string, float>);
	float get_custom_arg(string rules);
	void clear_custom_config();

	bool error_indicator = false;
protected:
	unordered_map<string, float> custom_config;
	bool has_custom_config = false;

	unordered_map<string, int> operator_priority;
	unordered_map<string, float(AlgArgAnalyzer::*)(float, float)> operator_conductors;
	unordered_map<string, float> args;
	unordered_map<string, vector<string>> rules;
	PipelineConfig* config = nullptr;

	AlgArgAnalyzer();
	vector<string> split(string str, char space = ' ');
	bool is_operator(string str);
	bool is_data(string str);
	// a是否比b优先级高
	bool is_operator_advancer(string a, string b);
	float cal(string a, string b, string op);
	// 从符号t中获取数据
	float get_data(string t);
	// 运算执行者
	float plus(float, float);
	float subtract(float, float);
	float mul(float, float);
	float div(float, float);
	float power(float, float);
};

