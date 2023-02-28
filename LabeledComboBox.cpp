#include "LabeledComboBox.h"

LabeledComboBox::LabeledComboBox(LabeledComboBoxResponder* responder,string tag)
{
	this->tag = tag;
	this->responder = responder;
	init();
	addComponent();
}

void LabeledComboBox::add_item(string item)
{
	combo_box->addItem(QString::fromLocal8Bit(item.c_str()));
}

void LabeledComboBox::on_streamed(void* v)
{
	string fv = *((string*)v);
	streamed = true;

	setDisabled(true);
	combo_box->setCurrentText(fv.c_str());
}

void LabeledComboBox::value_changed(void* v)
{
	if (streamed) {
		string fv = *((string*)v);
		combo_box->setCurrentText(fv.c_str());
	}
}

void LabeledComboBox::on_cutoff(void*)
{
	streamed = false;
	setDisabled(false);
}

void LabeledComboBox::set_node_data(NodeData<string>* node_data)
{
	this->node_data = node_data;
}

void LabeledComboBox::init()
{
	setStyleSheet(CssLoader::load_css("labeled_combo_box_style.css"));
	items = vector<string>();
}

void LabeledComboBox::addComponent()
{
	top_layout = new QHBoxLayout();
	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(5);

	UIModel* model = UIModel::get();
	setFixedHeight(model->float_slider_height);

	tag_label = new QLabel();
	tag_label->setText(QString::fromLocal8Bit(tag.c_str()));
	tag_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	top_layout->addWidget(tag_label);

	combo_box = new QComboBox();
	connect(combo_box, &QComboBox::currentTextChanged, [this](const QString& text) {
		responder->value_changed(tag, text.toStdString());
		if (node_data) {
			if (!streamed) {
				node_data->set(text.toStdString());
			}
		}
		});

	top_layout->addWidget(combo_box);
	
	setLayout(top_layout);
}
