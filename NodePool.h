#pragma once
#include"NodeData.h"
#include"IdGenerator.h"
#include<unordered_map>
#include<qcolor.h>
#include<qstring.h>
#include <time.h>
#include"LayerConfig.h"
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

	// reverse 表示用src_id当dst_id, 后者反之
	bool connect_(int src_id, int dst_id, bool reverse = false);

	template<typename T>
	bool cut_off(int src_id, int dst_id);

	// reverse 表示用src_id当dst_id， 后者反之
	bool cut_off_(int src_id, int dst_id, bool reverse = false);

	string get_value_type(int id);
protected:
	unordered_map<int, void*> data_pool;
	IdGenerator* generator;
};
inline NodePool::NodePool() {
	data_pool = unordered_map<int, void*>();
	generator = new IdGenerator(time(NULL), 1000);
}

inline bool NodePool::connect_(int src_id, int dst_id, bool reverse)
{
	string start_value = get_value_type(src_id);
	string stop_value = get_value_type(dst_id);
	if (start_value != stop_value) {
		return false;
	}
	if (start_value == "float") {
		if (!reverse) {
			return connect<float>(src_id, dst_id);
		}
		else {
			return connect<float>(dst_id, src_id);
		}
	}
	else if (start_value == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >") {
		if (!reverse) {
			return connect<float>(src_id, dst_id);
		}
		else {
			return connect<float>(dst_id, src_id);
		}
	}
	else if (start_value == "class QColor") {
		if (!reverse) {
			return connect<QColor>(src_id, dst_id);
		}
		else {
			return connect<QColor>(dst_id, src_id);
		}
	}
	else if (start_value == "class LayerConfig * __ptr64") {
		if (!reverse) {
			return connect<LayerConfig*>(src_id, dst_id);
		}
		else {
			return connect<LayerConfig*>(dst_id, src_id);
		}
	}
	else if (start_value == "class QString") {
		if (!reverse) {
			return connect<QString>(src_id, dst_id);
		}
		else {
			return connect<QString>(dst_id, src_id);
		}
	}
	return false;
}

inline bool NodePool::cut_off_(int src_id, int dst_id, bool reverse)
{
	string start_value = get_value_type(src_id);
	if (start_value == "float") {
		if (!reverse) {
			return cut_off<float>(src_id, dst_id);
		}
		else {
			return cut_off<float>(dst_id, src_id);
		}
	}
	else if (start_value == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >") {
		if (!reverse) {
			return cut_off<float>(src_id, dst_id);
		}
		else {
			return cut_off<float>(dst_id, src_id);
		}
	}
	else if (start_value == "class QColor") {
		if (!reverse) {
			return cut_off<QColor>(src_id, dst_id);
		}
		else {
			return cut_off<QColor>(dst_id, src_id);
		}
	}
	else if (start_value == "class LayerConfig * __ptr64") {
		if (!reverse) {
			return cut_off<LayerConfig*>(src_id, dst_id);
		}
		else {
			return cut_off<LayerConfig*>(dst_id, src_id);
		}
	}
	else if (start_value == "class QString") {
		if (!reverse) {
			return cut_off<QString>(src_id, dst_id);
		}
		else {
			return cut_off<QString>(dst_id, src_id);
		}
	}
	return false;
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
