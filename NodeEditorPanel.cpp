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

	add_btn(NODE_EDITOR_PANEL_PERLIN_LAYER, "perlin_layer");
	add_btn(NODE_EDITOR_PANEL_DPD_LAYER, "dpd_layer");
	add_btn(NODE_EDITOR_PANEL_LAYER_MACHINE, "layer_machine");
	add_btn(NODE_EDITOR_PANEL_SAVE_CONFIG, "save_config");
	add_btn(NODE_EDITOR_PANEL_READ_CONFIG, "read_config");

	setLayout(top_layout);
}

void NodeEditorPanel::add_btn(QString title, string tag)
{
	QPushButton* btn = new QPushButton();

	btn->setText(title);
	connect(btn, &QPushButton::clicked, [this, tag](){
		if (responder) {
			responder->btn_down(tag);
			}
		
		});

	top_layout->addWidget(btn);
}
