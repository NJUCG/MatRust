#include "AlgArgAnalyzer.h"

AlgArgAnalyzer* AlgArgAnalyzer::analyzer = nullptr;

AlgArgAnalyzer::AlgArgAnalyzer()
{
    operator_priority["+"] = 0;
    operator_priority["-"] = 0;
    operator_priority["*"] = 1;
    operator_priority["/"] = 1;
    operator_priority["^"] = 2;

    operator_conductors["+"] = &AlgArgAnalyzer::plus;
    operator_conductors["-"] = &AlgArgAnalyzer::subtract;
    operator_conductors["*"] = &AlgArgAnalyzer::mul;
    operator_conductors["/"] = &AlgArgAnalyzer::div;
    operator_conductors["^"] = &AlgArgAnalyzer::power;

    (this->*operator_conductors["+"])(1, 1);
}

void AlgArgAnalyzer::init_analyzer()
{
    analyzer = new AlgArgAnalyzer();
}

float AlgArgAnalyzer::get_arg(string rules)
{
    if (!config) {
        return 0.0f;
    }
    error_indicator = false;

    vector<string> elems = split(rules);
    stack<string> data;
    stack<string> operators;
    for (string elem : elems) {
        if (is_operator(elem)) {
            if (operators.empty() || operators.top() == "(") {
                operators.push(elem);
            }
            else {
                string prev;
                if (operators.empty()) {
                    operators.push(elem);
                }
                else {
                    prev = operators.top();
                    while (is_operator_advancer(prev, elem) && !operators.empty())
                    {
                        operators.pop();

                        string b = data.top();
                        data.pop();
                        string a = data.top();
                        data.pop();
                        data.push(std::to_string(cal(a, b, prev)));
                        if (operators.empty()) {
                            break;
                        }
                    } 
                    operators.push(elem);
                }
            }
        }
        else if(elem == "(") {
            operators.push(elem);
        }
        else if (elem == ")") {
            while (operators.top() != "(") {
                string prev = operators.top();
                operators.pop();
                string b = data.top();
                data.pop();
                string a = data.top();
                data.pop();
                data.push(std::to_string(cal(a, b, prev)));
            }
            operators.pop();
        }
        else if (is_data(elem)) {
            data.push(elem);
        }
        else {
            error_indicator = true;
            return 0;
        }
    }

    while (!operators.empty()) {
        string prev = operators.top();
        operators.pop();
        string b = data.top();
        data.pop();
        string a = data.top();
        data.pop();
        data.push(std::to_string(cal(a, b, prev)));
    }

    return stof(data.top());
}

void AlgArgAnalyzer::register_config(PipelineConfig* config)
{
    this->config = config;
}

vector<string> AlgArgAnalyzer::split(string str, char space)
{
    stringstream ss(str);
    string item;
    vector<string> elems;
    while (getline(ss, item, space)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}

bool AlgArgAnalyzer::is_operator(string str)
{
    return operator_priority.find(str) != operator_priority.end();
}
bool AlgArgAnalyzer::is_data(string str) {
    if (!has_custom_config && !config) {
        return false;
    }
    if (config) {
        if (str == "oc" || str == "rh" || str == "sc" || str == "cc" || str == "T") {
            return true;
        }
        stringstream ss(str);
        float ret = 0.0f;
        if (ss >> ret) {
            return true;
        }
        return false;
    }
    if (has_custom_config) {
        if (custom_config.find(str) != custom_config.end()) {
            return true;
        }
        stringstream ss(str);
        float ret = 0.0f;
        if (ss >> ret) {
            return true;
        }
        return false;
    }
}
// a是否比b优先级高
bool AlgArgAnalyzer::is_operator_advancer(string a, string b) {
    int ap = operator_priority[a];
    int bp = operator_priority[b];
    return operator_priority[a] >= operator_priority[b];
}
float AlgArgAnalyzer::cal(string sa, string sb, string op) {

    float a = get_data(sa);
    float b = get_data(sb);
    return (this->*operator_conductors[op])(a, b);
}

float AlgArgAnalyzer::get_data(string t)
{
    if (!has_custom_config && !config) {
        return 0.0f;
    }
    if (config) {
        if (t == "oc") {
            return config->oc / 100.0f;
        }
        else if (t == "sc") {
            return config->sc / 100.0f;
        }
        else if (t == "rh") {
            return config->rh / 100.0f;
        }
        else if (t == "cc") {
            return config->cc / 100.0f;
        }
        else if (t == "T") {
            return config->temperature;
        }
        stringstream ss(t);
        float ret = 0.0f;
        if (!(ss >> ret)) {
            return 0.0f;
        }
        return ret;
    }
    if (has_custom_config) {
        if (custom_config.find(t) != custom_config.end()) {
            return custom_config[t];
        }
        stringstream ss(t);
        float ret = 0.0f;
        if (!(ss >> ret)) {
            return 0.0f;
        }
        return ret;
    }
}

float AlgArgAnalyzer::plus(float a, float b)
{
    return a + b;
}

float AlgArgAnalyzer::subtract(float a, float b)
{
    return a - b;
}
float AlgArgAnalyzer::mul(float a, float b) {
    return a * b;
}
float AlgArgAnalyzer::div(float a, float b) {
    return a / (b + 0.001f);
}
float AlgArgAnalyzer::power(float a, float b) {
    return pow(a, b);
}
float AlgArgAnalyzer::get_custom_arg(string rules) {
    if (!has_custom_config) {
        return 0.0f;
    }
    error_indicator = false;

    vector<string> elems = split(rules);
    stack<string> data;
    stack<string> operators;
    for (string elem : elems) {
        if (is_operator(elem)) {
            if (operators.empty() || operators.top() == "(") {
                operators.push(elem);
            }
            else {
                string prev;
                if (operators.empty()) {
                    operators.push(elem);
                }
                else {
                    prev = operators.top();
                    while (is_operator_advancer(prev, elem) && !operators.empty())
                    {
                        operators.pop();

                        string b = data.top();
                        data.pop();
                        string a = data.top();
                        data.pop();
                        data.push(std::to_string(cal(a, b, prev)));

                        prev = operators.top();
                    }
                    operators.push(elem);
                }
            }
        }
        else if (elem == "(") {
            operators.push(elem);
        }
        else if (elem == ")") {
            while (operators.top() != "(") {
                string prev = operators.top();
                operators.pop();
                string b = data.top();
                data.pop();
                string a = data.top();
                data.pop();
                data.push(std::to_string(cal(a, b, prev)));
            }
            operators.pop();
        }
        else if (is_data(elem)) {
            data.push(elem);
        }
        else {
            error_indicator = true;
            return 0;
        }
    }

    while (!operators.empty()) {
        string prev = operators.top();
        operators.pop();
        string b = data.top();
        data.pop();
        string a = data.top();
        data.pop();
        data.push(std::to_string(cal(a, b, prev)));
    }

    return stof(data.top());
}
void AlgArgAnalyzer::clear_custom_config() {
    has_custom_config = false;
}
void AlgArgAnalyzer::clear_config() {
    config = nullptr;
}
void AlgArgAnalyzer::register_custom_config(unordered_map<string, float> c) {
    custom_config = c;
    has_custom_config = true;
}