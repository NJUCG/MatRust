#pragma once
#include<string>
#include<unordered_map>
#include<vector>
#include"EventResponder.h"
#include<stack>
#include"qwidget.h"
using namespace std;

class EventAdapter
{
public:
	static EventAdapter* shared;
	void creat_event(string);
	void register_event(string, EventResponder*);
	void deregsiter_event(string, EventResponder*);
	void trigger_event(string);
	void push_data(void*);
	void* pop_data();
	static void init_event_adapter();
protected:
	stack<void*> event_stack;
	unordered_map<string, vector<EventResponder*>> events;
	EventAdapter();
};

