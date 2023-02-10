#include "EventAdapter.h"
EventAdapter* EventAdapter::shared = nullptr;
void EventAdapter::creat_event(string name) {
	if (events.find(name) == events.end()) {
		events[name] = vector<EventResponder*>();
	}
}
void EventAdapter::register_event(string name, EventResponder* responder) {
	if (events.find(name) == events.end()) {
		creat_event(name);
	}
	if (find(events[name].begin(), events[name].end(), responder) == events[name].end()) {
		events[name].push_back(responder);
	}
}
void EventAdapter::deregsiter_event(string name, EventResponder* responder) {
	if (events.find(name) != events.end()) {
		if (find(events[name].begin(), events[name].end(), responder) != events[name].end()) {
			remove(events[name].begin(), events[name].end(), responder);
		}
	}
}
void EventAdapter::trigger_event(string name) {
	if (events.find(name) != events.end()) {
		for (vector<EventResponder*>::iterator it = events[name].begin(); it != events[name].end(); it++) {
			(*it)->on_trigger(name);
		}
	}
}
void EventAdapter::init_event_adapter() {
	shared = new EventAdapter();
}
void EventAdapter::push_data(void* data) {
	event_stack.push(data);
}
void* EventAdapter::pop_data() {
	void* ret = nullptr;
	if (event_stack.size() > 0) {
		ret = event_stack.top();
		event_stack.pop();
	}
	return ret;
}
EventAdapter::EventAdapter() {
	event_stack = stack<void*>();
}