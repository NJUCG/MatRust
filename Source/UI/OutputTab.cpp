#include "OutputTab.h"

OutputTab::OutputTab()
{
	init();
	addComponent();
}

void OutputTab::on_trigger(string)
{
}

void OutputTab::on_switch_to()
{
}

void OutputTab::init()
{
	setStyleSheet(CssLoader::load_css("output_tab_style.css"));
}

void OutputTab::addComponent()
{
	top_layout = new QVBoxLayout();
	header(OUTPUT_TAB_HEADER);
	body();

	QWidget* view_mode_widget = new QWidget();
	QHBoxLayout* view_mode_layout = new QHBoxLayout();
	view_mode_layout->setContentsMargins(0, 0, 0, 0);
	view_mode_layout->setSpacing(5);


	view_mode_widget->setLayout(view_mode_layout);

	add_pic("", "");
	end_body();
	setLayout(top_layout);
}

void OutputTab::header(QString header)
{
	QWidget* head = new QWidget();

	UIModel* model = UIModel::get();
	head->setFixedHeight(model->control_panel_header_height);

	head->setObjectName("head");

	QHBoxLayout* header_layout = new QHBoxLayout();
	header_layout->setContentsMargins(0, 0, 0, 0);

	QPushButton* icon_btn = new QPushButton();
	icon_btn->setObjectName("header_icon");
	icon_btn->setStyleSheet("background-color:transparent;color:transparent;border:solid 0px;");

	QIcon* icon = new QIcon("resources/ui/icons/output-30.png");

	icon_btn->setIcon(*icon);
	icon_btn->setIconSize(QSize(model->control_selector_icon_width, model->control_selector_icon_height));
	icon_btn->setFixedSize(QSize(model->control_selector_button_width, model->control_selector_button_height));
	icon_btn->setEnabled(false);

	header_layout->addWidget(icon_btn);

	QLabel* value_label = new QLabel();
	value_label->setText(header);
	value_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	header_layout->addWidget(value_label);

	header_layout->addWidget(new QWidget());

	head->setLayout(header_layout);

	top_layout->addWidget(head);
}

void OutputTab::body()
{
	body_widget = new QWidget();
	body_layout = new QVBoxLayout();
	body_layout->setContentsMargins(0, 0, 0, 0);

	body_widget->setLayout(body_layout);
	top_layout->addWidget(body_widget);
}

void OutputTab::add_pic(QString title,string tag)
{
	QWidget* line_widget = new QWidget();
	QHBoxLayout* line_layout = new QHBoxLayout();

	QWidget* pic_container = new QWidget();
	QVBoxLayout* container_layout = new QVBoxLayout();

	QLabel* pic_widget = new QLabel();
	pic_widget->setBackgroundRole(QPalette::Base);
	pic_widget->setScaledContents(true);
	container_layout->addWidget(pic_widget);
	imgs[tag] = pic_widget;
	
	pic_container->setLayout(container_layout);

	line_widget->setLayout(line_layout);

	body_layout->addWidget(line_widget);
}


void OutputTab::end_body()
{
	body_layout->addWidget(new QWidget());
}

void OutputTab::show_growth()
{
}

void OutputTab::hide_growth()
{
}
