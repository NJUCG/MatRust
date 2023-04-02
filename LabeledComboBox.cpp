#include "LabeledComboBox.h"

LabeledComboBox::LabeledComboBox(LabeledComboBoxResponder* responder,QString tag)
{
	this->tag = tag;
	this->responder = responder;
	init();
	addComponent();
}

void LabeledComboBox::add_item(QString item)
{
	combo_box->addItem(item);
}

void LabeledComboBox::on_streamed(void* v,int)
{
	QString fv = *((QString*)v);
	streamed = true;

	setDisabled(true);
	combo_box->setCurrentText(fv);
}

void LabeledComboBox::value_changed(void* v,int)
{
	if (streamed) {
		QString fv = *((QString*)v);
		combo_box->setCurrentText(fv);
	}
}

void LabeledComboBox::on_cutoff(void*,int)
{
	streamed = false;
	setDisabled(false);
}

void LabeledComboBox::set_node_data(NodeData<QString>* node_data)
{
	this->node_data = node_data;
}

void LabeledComboBox::init()
{
	setStyleSheet(CssLoader::load_css("labeled_combo_box_style.css"));
	items = vector<QString>();
}

void LabeledComboBox::addComponent()
{
	top_layout = new QHBoxLayout();
	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(5);

	UIModel* model = UIModel::get();
	setFixedHeight(model->float_slider_height);

	tag_label = new QLabel();
	tag_label->setText(tag);
	tag_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	top_layout->addWidget(tag_label);

	combo_box = new QComboBox();
	connect(combo_box, &QComboBox::currentTextChanged, [this](const QString& text) {
		responder->value_changed(tag, text);
		if (node_data) {
			if (!streamed) {
				node_data->set(text);
			}
		}
		});

	top_layout->addWidget(combo_box);
	
	setLayout(top_layout);
}
