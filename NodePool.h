#pragma once
#include"NodeData.h"
#include"IdGenerator.h"
#include<unordered_map>
#include <time.h>
class NodePool
{
public:
	NodePool();
	
	template<typename T>
	int creat();

	template<typename T>
	bool rm(int id);

	template<typename T>
	NodeData<T>* get_data(int id);

	template<typename T>
	void set_value(int id, T value);

	template<typename T>
	T get_value(int id);

	template<typename T>
	bool connect(int src_id, int dst_id);

	template<typename T>
	bool cut_off(int src_id, int dst_id);

	string get_value_type(int id);
protected:
	unordered_map<int, void*> data_pool;
	IdGenerator* generator;
};
inline NodePool::NodePool() {
	data_pool = unordered_map<int, void*>();
	generator = new IdGenerator(time(NULL), 1000);
}

inline string NodePool::get_value_type(int id)
{
	return ((NodeDataAdapter*)data_pool[id])->type;
}

template<typename T>
inline int NodePool::creat() {
	int id = generator->next();
	NodeData<T>* data = new NodeData<T>();
	data->id = id;
	data_pool[id] = data;
	data->type = (typeid(T)).name();
	return id;
}
template<typename T>
inline T NodePool::get_value(int id) {
	NodeData<T>* dst = (NodeData<T>*)data_pool[id];
	return dst->get();
}
template<typename T>
inline void NodePool::set_value(int id, T value) {
	NodeData<T>* dst = (NodeData<T>*)data_pool[id];
	return dst->set(value);
}
template<typename T>
inline bool NodePool::rm(int id) {
	if (data_pool.find(id) == data_pool.end()) {
		return false;
	}
	else {
		data_pool[id] = nullptr;
		return true;
	}
}

template<typename T>
inline NodeData<T>* NodePool::get_data(int id) {
	if (data_pool.find(id) == data_pool.end()) {
		return nullptr;
	}
	else {
		return (NodeData<T>*)(data_pool[id]);
	}
}

template<typename T>
inline bool NodePool::connect(int src_id, int dst_id) {
	NodeData<T>* src = (NodeData<T>*)data_pool[src_id];
	NodeData<T>* dst = (NodeData<T>*)data_pool[dst_id];
	return src->add_adapter(dst);
}

template<typename T>
inline bool NodePool::cut_off(int src_id, int dst_id) {
	NodeData<T>* src = (NodeData<T>*)data_pool[src_id];
	NodeData<T>* dst = (NodeData<T>*)data_pool[dst_id];
	src->shut_off(dst);
	return true;
}
