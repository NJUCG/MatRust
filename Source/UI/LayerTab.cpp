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
}

void LayerTab::addComponent()
{
	top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(0);

	editor = new NodeEditor();
	top_layout->addWidget(editor);

	setLayout(top_layout);
}
