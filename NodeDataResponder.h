#pragma once
class NodeDataResponder
{
public:
	virtual void on_streamed(void*);
	virtual void value_changed(void*);
	virtual void on_cutoff(void*);
};

