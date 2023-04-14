#pragma once
class NodeDataResponder
{
public:
	virtual void on_streamed(void*, int);
	virtual void value_changed(void*,int);
	virtual void on_cutoff(void*,int);
};

