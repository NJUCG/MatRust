#include "LayerTab.h"

LayerTab::LayerTab()
{
	init();
	addComponent();
}

LayerTab::~LayerTab()
{
	if (top_layout) {
		delete top_layout;
	}
	if (editor) {
		delete editor;
	}
}

void LayerTab::init()
{
	setStyleSheet("background-color:#3D3D3D;");
}

void LayerTab::addComponent()
{
	top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(0, 5, 0, 5);
	top_layout->setSpacing(0);

	header();

	editor = new NodeEditor();
	top_layout->addWidget(editor);

	setLayout(top_layout);
}

void LayerTab::header()
{
	QWidget* head = new QWidget();

	UIModel* model = UIModel::get();
	head->setFixedHeight(model->control_selector_button_height);

	head->setObjectName("head");
	head->setStyleSheet("background-color: transparent;color:white;border:0px solid red;");

	QHBoxLayout* header_layout = new QHBoxLayout();
	header_layout->setContentsMargins(10, 0, 10, 0);
	header_layout->setSpacing(5);

	QPushButton* icon_btn = new QPushButton();
	icon_btn->setObjectName("header_icon");
	icon_btn->setStyleSheet("background-color: transparent;color:transparent;border:0px solid;");

	QIcon* icon = new QIcon("resources/ui/icons/layer-30.png");

	icon_btn->setIcon(*icon);
	icon_btn->setIconSize(QSize(20, 20));
	icon_btn->setFixedSize(QSize(20, 20));
	icon_btn->setEnabled(false);

	header_layout->addWidget(icon_btn);

	QLabel* value_label = new QLabel();
	value_label->setText(LAYER_TAB_TITLE);
	value_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	header_layout->addWidget(value_label);

	header_layout->addWidget(new QWidget());

	head->setLayout(header_layout);

	top_layout->addWidget(head);
}
