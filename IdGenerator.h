#pragma once
#include<list>
using namespace std;
class IdGenerator
{
public:
	IdGenerator(int seed,int count) {
		id_pool = list<int>();
		int start = seed % count;
		for (int i = 0; i < count; i++) {
			id_pool.push_front((start + i) % count);
		}
	}
	int next() {
		int t = id_pool.back();
		id_pool.pop_back();
		id_pool.push_front(t);
		return t;
	}
protected:
	list<int> id_pool;
};

