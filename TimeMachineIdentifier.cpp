#include "TimeMachineIdentifier.h"

TimeMachineIdentifier::TimeMachineIdentifier(float start_time,float stop_time,float step)
{
	this->start_time = start_time;
	this->stop_time = stop_time;
	this->step = step;
	init();
	addComponent();
}

void TimeMachineIdentifier::register_responder(TimeMachineResponder* responder)
{
	events.push_back(responder);
}

TimeMachineIdentifier::~TimeMachineIdentifier()
{
	delete timer;
}

void TimeMachineIdentifier::play()
{
	timer->start(50);

	PipelineManager::shared->start_pipeline();
}

void TimeMachineIdentifier::pause()
{
	timer->stop();
}

void TimeMachineIdentifier::clear()
{
	timer->stop();
	current_time = 0;
	update();
}

void TimeMachineIdentifier::init()
{
	events = vector<TimeMachineResponder*>();

	setStyleSheet(CssLoader::load_css("time_machine_identifier.css"));

	tag_to_show = ((stop_time - start_time) / step) + 1;

	current_time = 0;

	float msc = 0.05;
	timer = new QTimer();
	connect(timer, &QTimer::timeout, [this, msc]()->void {
		current_time += msc;
		block_only = true;

		if (current_time > stop_time) {
			current_time = stop_time;
			pause();
			int rs = events.size();
			for (int i = 0; i < rs; i++) {
				TimeMachineResponder* responder = events[i];
				if (responder) {
					responder->time_up();
				}
			}
			emit time_exhausted();
		}

		int rs = events.size();
		for (int i = 0; i < rs; i++) {
			TimeMachineResponder* responder = events[i];
			if (!responder->pause) {
				responder->frame_consumer(msc);
			}
		}

		PipelineManager::shared->generate_rust(msc);

		update();
		});
}

void TimeMachineIdentifier::addComponent()
{
	setFixedHeight(40);

}

void TimeMachineIdentifier::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);

	QStyleOption opt;
	opt.initFrom(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

	int start_x = 5;
	QColor color(165, 165, 165);
	int h = height();
	for (float t = start_time; t <= stop_time; t += step) {
		QRectF rect(start_x, 5, 40, 30);

		painter.setPen(color);
		painter.drawText(rect, Qt::AlignHCenter | Qt::AlignVCenter, to_string(t).c_str());

		QLine current_line;
		current_line.setP1(QPoint(start_x + 40 / 2, h));
		current_line.setP2(QPoint(start_x + 40 / 2, h - 5));
		painter.drawLine(current_line);

		start_x += 40;
		start_x += interval;
	}
	// 移动滑块
	// 获取滑块位置
	int current_loc = machine_margin + dist_per_unit * (current_time - start_time);
	QRectF rect(current_loc, 5, 40, 30);
	QPen pen;
	pen.setWidth(0);
	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	QColor blue(66, 109, 174);
	brush.setColor(blue);
	painter.setPen(pen);
	painter.setBrush(brush);
	painter.drawRoundedRect(rect, 4, 4);

	QLine line(QPoint(current_loc + 20, h), QPoint(current_loc + 20, h - 5));
	pen = QPen();
	pen.setColor(blue);
	painter.setPen(pen);
	painter.drawLine(line);

	painter.setPen(color);
	painter.drawText(rect, Qt::AlignHCenter | Qt::AlignVCenter, to_string_by(current_time).c_str());
}

void TimeMachineIdentifier::resizeEvent(QResizeEvent*)
{
	int current_width = width();
	interval = (current_width - 2 * machine_margin - 40 * tag_to_show) / (tag_to_show - 1);
	dist_per_unit = (current_width - 2 * machine_margin - 40) / (stop_time - start_time);
}

string TimeMachineIdentifier::to_string(float value)
{
	stringstream ss;
	ss << std::setiosflags(std::ios::fixed) << (int)value;
	return string(ss.str());
}

string TimeMachineIdentifier::to_string_by(float value, int prec)
{
	stringstream ss;
	ss << std::setiosflags(std::ios::fixed) << std::setprecision(prec) << value;
	return string(ss.str());
}
