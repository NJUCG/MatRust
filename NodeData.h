#pragma once
#include"NodeObject.h"
#include<vector>
#include"NodeDataAdapter.h"
#include"NodeDataResponder.h"
using namespace std;
template <class T>
class NodeData:public NodeDataAdapter
{
public:
	NodeData();
	int id = -1;
	// 输入
	NodeData* source;
	// 输出
	vector<NodeData*> adapter;
	// 响应函数
	// 你老兄切一下某个adapter
	void shut_off(NodeData*);
	// 你老兄多送一点数据
	bool add_adapter(NodeData*);
	T* value;
	T get();
	void set(T);
	void add_responder(NodeDataResponder*);
protected:
	vector<NodeDataResponder*> responder;
	void init();
	// source断流了
	void on_shut_off();
	// 链接到某个source
	bool stream_from(NodeData*);
	void on_set_value();
};

template<class T>
NodeData<T>::NodeData() {
	init();
}
template<class T>
void NodeData<T>::add_responder(NodeDataResponder* responder) {
	this->responder.push_back(responder);
}
template<class T>
void NodeData<T>::init() {
	source = nullptr;
	adapter = vector<NodeData<T>*>();
	value = nullptr;
	responder = vector<NodeDataResponder*>();
}
template<class T>
void NodeData<T>::on_shut_off() {
	this->source = nullptr;
	this->value = nullptr;
	for (size_t i = 0; i < adapter.size(); i++)
	{
		adapter[i]->stream_from(this);
	}
	for (int i = 0; i < responder.size(); i++) {
		responder[i]->on_cutoff(nullptr);
	}
}
template<class T>
void NodeData<T>::shut_off(NodeData* a) {
	int erase_index = -1;
	for (size_t i = 0; i < adapter.size(); i++)
	{
		if (adapter[i] == a) {
			erase_index = i;
			break;
		}
	}
	if (erase_index >= 0) {
		adapter[erase_index]->on_shut_off();
		adapter.erase(adapter.begin() + erase_index);
	}
}
template<class T>
bool NodeData<T>::stream_from(NodeData* src) {
	if (src->value == value && (src->value || value)) {
		return false;
	}
	source = src;
	value = source->value;
	for (size_t i = 0; i < adapter.size(); i++)
	{
		adapter[i]->stream_from(this);
	}
	for (int i = 0; i < responder.size(); i++) {
		responder[i]->on_streamed(value);
	}
	return true;
}
template<class T>
T NodeData<T>::get() {
	if (value != nullptr)
		return *value;
	else {
		return T();
	}
}
template<class T>
void NodeData<T>::set(T var) {
	if (source) {
		return;
	}
	if (value != nullptr)
		*value = var;
	else {
		value = new T();
		*value = var;
		for (size_t i = 0; i < adapter.size(); i++)
		{
			adapter[i]->stream_from(this);
		}
	}
	on_set_value();
	for (size_t i = 0; i < adapter.size(); i++)
	{
		adapter[i]->on_set_value();
	}
}
template<class T>
void NodeData<T>::on_set_value() {
	for (int i = 0; i < responder.size(); i++) {
		responder[i]->value_changed(value);
	}
}
template<class T>
bool NodeData<T>::add_adapter(NodeData* adp) {
	if (std::count(adapter.begin(), adapter.end(), adp) == 0) {
		adapter.push_back(adp);
		if (value == nullptr) {
			value = new T();
		}
		adp->stream_from(this);
		return true;
	}
	else {
		return false;
	}
}