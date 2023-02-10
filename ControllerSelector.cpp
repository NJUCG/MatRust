#include "ControllerSelector.h"
ControllerSelector::ControllerSelector(ControllSelectorDelegate* delegator)
{
	init();
	this->delegator = delegator;
	addComponent();
}
void ControllerSelector::init()
{
	model = UIModel::get();
	selector_dict = unordered_map<string, QPushButton*>();
	setFixedWidth(model->control_selecotr_width);
	setStyleSheet(CssLoader::load_css("control_selector.css"));
}
void ControllerSelector::addComponent()
{
	QVBoxLayout* layout = new QVBoxLayout();
	QMargins box_margin(5, 0, 0, 0);
	layout->setContentsMargins(box_margin);
	layout->setSpacing(3);
	int line_count = 10;
	int item_count = 3;
	QWidget* margin = new QWidget();
	addPanelButton("bake", layout);
	addPanelButton("env", layout);
	addPanelButton("engine-30", layout);
	addPanelButton("object-30", layout);
	layout->addWidget(margin);
	for (int i = 0; i < item_count; i++) {
		layout->setStretch(i, 1);
	}
	layout->setStretch(item_count, line_count - item_count);
	setLayout(layout);
}
void ControllerSelector::addPanelButton(string type, QVBoxLayout* layout)
{
	QPushButton* push_button = new QPushButton();
	push_button->setObjectName(type);
	QIcon* icon = new QIcon((string("resources/ui/icons/")+ type + string(".png")).c_str());
	push_button->setIcon(*icon);
	push_button->setIconSize(QSize(model->control_selector_icon_width, model->control_selector_icon_height));
	push_button->setFixedSize(QSize(model->control_selector_button_width, model->control_selector_button_height));
	push_button->setCheckable(true);
	push_button->setAutoExclusive(true);
	connect(push_button, &QPushButton::clicked, [this, type]()->void {
		delegator->switchTo(type);
		});
	layout->addWidget(push_button);
	selector_dict[type] = push_button;
}
