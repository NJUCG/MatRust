#include "NodeEditorPanel.h"

NodeEditorPanel::NodeEditorPanel()
{
	init();
	addComponent();
}

NodeEditorPanel::~NodeEditorPanel()
{
	if (top_layout) {
		delete top_layout;
	}
}

void NodeEditorPanel::init()
{
	setStyleSheet(CssLoader::load_css("node_editor_panel_style.css"));
}

void NodeEditorPanel::addComponent()
{
	top_layout = new QVBoxLayout();

	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(0);

	add_btn("custom layer", "custom_layer");
	add_btn("layer machine", "layer_machine");
	add_btn("normal", "no");

	setLayout(top_layout);
}

void NodeEditorPanel::add_btn(string title, string tag)
{
	QPushButton* btn = new QPushButton();

	btn->setText(QString::fromLocal8Bit(title.c_str()));
	connect(btn, &QPushButton::clicked, [this, tag](){
		if (responder) {
			responder->btn_down(tag);
			}
		
		});

	top_layout->addWidget(btn);
}
