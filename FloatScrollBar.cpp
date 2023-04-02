#include "FloatScrollBar.h"

FloatSliderBar::FloatSliderBar(FloatSliderBarResponder* responder, QString title, float min, float max,float delta)
{
	this->responder = responder;
	this->delta = delta;
	this->title = title;
	setOrientation(Qt::Horizontal);
	setMinimum(min);
	setMaximum(max);
	setValue(0);
	init();
	addComponent();
}

float FloatSliderBar::d_value()
{
	return (float)value() / delta;
}

void FloatSliderBar::on_streamed(void* v,int)
{
	float fv = *((float*)v);
	streamed = true;

	setDisabled(true);
	setValue(fv * delta);
}

void FloatSliderBar::value_changed(void* v, int)
{
	float fv = *((float*)v);
	setValue(fv * delta);
}

void FloatSliderBar::on_cutoff(void*,int)
{
	streamed = false;
	setDisabled(false);
}

void FloatSliderBar::set_node_data(NodeData<float>* d)
{
	node_data = d;
}

void FloatSliderBar::init()
{
	setStyleSheet(CssLoader::load_css("float_progressbar.css"));
}

void FloatSliderBar::addComponent()
{
	QHBoxLayout* layout = new QHBoxLayout();
	UIModel* model = UIModel::get();

	setFixedHeight(model->float_slider_height);

	name_label = new QLabel();
	//name_label->setFixedWidth(model->float_slider_title_label_width);
	name_label->setText(title);

	layout->addWidget(name_label);

	value_label = new QLabel();
	value_label->setFixedWidth(model->float_slider_value_label_width);
	value_label->setText("0.00");
	connect(this, &QSlider::valueChanged,this, &FloatSliderBar::onValueChanged);
	layout->addWidget(value_label);

	setLayout(layout);
}

void FloatSliderBar::onValueChanged(int value)
{
	stringstream ss;
	float d_value = ((float)value) / delta;
	ss << std::setiosflags(std::ios::fixed) << std::setprecision(2) << d_value;
	string txt(ss.str());
	value_label->setText(QString::fromLocal8Bit(txt.c_str()));
	if(responder)
		responder->onValueChanged(d_value);
	if (node_data) {
		if (!streamed) {
			node_data->set(d_value);
		}
	}
}
