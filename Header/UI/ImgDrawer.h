#pragma once
#include"EventAdapter.h"
#include<qlabel.h>
#include<qpainter.h>
#include<glm/glm.hpp>
#include<vector>
using namespace std;
using namespace glm;

class ImgDrawer:public QLabel
{
public:
	enum Policy {
		Single,
		Repeat,
	};
	ImgDrawer(int h, int w, vector<vector<vec4>>* img) {
		this->w = w;
		this->h = h;
		this->img = img;
		this->painterFunc = &ImgDrawer::paint_it_repeat;
	}
	void setPolicy(Policy policy) {
		this->policy = policy;
		if (policy == Policy::Single) {
			this->painterFunc = &ImgDrawer::paint_it_single;
		}
		else if (policy == Policy::Repeat) {
			this->painterFunc = &ImgDrawer::paint_it_repeat;
		}
	}
	Policy switchPolicy() {
		policy = (Policy)(((int)policy + 1) % 2);
		if (policy == Policy::Single) {
			this->painterFunc = &ImgDrawer::paint_it_single;
		}
		else if (policy == Policy::Repeat) {
			this->painterFunc = &ImgDrawer::paint_it_repeat;
		}
		return policy;
	}
	ImgDrawer() {
		this->painterFunc = &ImgDrawer::paint_it_repeat;
	}
	void reset_img(int h, int w, vector<vector<vec4>>* img) {
		this->w = w;
		this->h = h;
		this->img = img;
	}
	void paintEvent(QPaintEvent* e) override {
		if (!img) {
			QLabel::paintEvent(e);
		}
		else {
			QPainter painter(this);
			(this->*painterFunc)(painter);
		}
	}
	void save_pic(QString loc) {
		EventAdapter::shared->push_data(new QString(loc));
		EventAdapter::shared->push_data(img);
		EventAdapter::shared->trigger_event("save_custom_img");
	}
private:
	vector<vector<vec4>>* img = nullptr;
	int w, h;
	Policy policy = Policy::Repeat;


	void (ImgDrawer::*painterFunc)(QPainter&) = nullptr;

	void paint_it_single(QPainter& painter){
		QColor color(255, 165, 165);
		painter.setPen(color);
		float iw = width();
		float ih = height();
		for (float i = 0; i < ih; i++) {
			for (float j = 0; j < iw; j++) {
				int ipos = (i / ih) * h;
				int jpos = (j / iw) * w;
				painter.setPen(QColor((*img)[ipos][jpos].r, (*img)[ipos][jpos].g, (*img)[ipos][jpos].b));
				painter.drawPoint(j, i);
			}
		}
	}

	void paint_it_repeat(QPainter& painter) {
		float iw = width();
		float ih = height();
		float scl = (float)ih / (float)h;
		int pw = w * scl;
		for (float i = 0; i < ih; i++) {
			for (float j = 0; j < iw; j++) {
				int ipos = i / scl;
				int jpos = ((int)j % pw) / scl;
				if (ipos >= h) {
					ipos = h - 1;
				}
				if (jpos >= w) {
					jpos = w;
				}
				painter.setPen(QColor((*img)[ipos][jpos].r, (*img)[ipos][jpos].g, (*img)[ipos][jpos].b));
				painter.drawPoint(j, i);
			}
		}
	}
};

