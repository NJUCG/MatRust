#include "OutputTab.h"

OutputTab::OutputTab()
{
	init();
	addComponent();
}

void OutputTab::on_trigger(string e)
{
	if (e == "merge_layers_finished") {
		imgs["diffuse"]->update();
		imgs["roughness"]->update();
		imgs["metallic"]->update();
		imgs["normal_disturb"]->update();
		imgs["height"]->update();
	}
	else if (e == "pipeline_started") {
		config = (PipelineConfig*)EventAdapter::shared->top_data();
	}
	else if (e == "diffuse_data_inited") {
		imgs["diffuse"]->reset_img(config->textureHeight, config->textureWidth, (vector<vector<vec4>>*)(EventAdapter::shared->pop_data()));
	}
	else if (e == "roughness_data_inited") {
		imgs["roughness"]->reset_img(config->textureHeight, config->textureWidth, (vector<vector<vec4>>*)(EventAdapter::shared->pop_data()));
	}
	else if (e == "metallic_data_inited") {
		imgs["metallic"]->reset_img(config->textureHeight, config->textureWidth, (vector<vector<vec4>>*)(EventAdapter::shared->pop_data()));
	}
	else if (e == "normal_disturb_data_inited") {
		imgs["normal_disturb"]->reset_img(config->textureHeight, config->textureWidth, (vector<vector<vec4>>*)(EventAdapter::shared->pop_data()));
	}
	else if (e == "height_data_inited") {
		imgs["height"]->reset_img(config->textureHeight, config->textureWidth, (vector<vector<vec4>>*)(EventAdapter::shared->pop_data()));
	}
}

void OutputTab::on_switch_to(int,int)
{
}

void OutputTab::resizeEvent(QResizeEvent* e)
{
	QWidget::resizeEvent(e);
}

void OutputTab::init()
{
	setStyleSheet(CssLoader::load_css("output_tab_style.css"));
	EventAdapter::shared->register_event("pipeline_started", this);
	EventAdapter::shared->register_event("merge_layers_finished", this);
	EventAdapter::shared->register_event("diffuse_data_inited", this);
	EventAdapter::shared->register_event("roughness_data_inited", this);
	EventAdapter::shared->register_event("metallic_data_inited", this);
	EventAdapter::shared->register_event("normal_disturb_data_inited", this);
	EventAdapter::shared->register_event("height_data_inited", this);

	repeat_icon = QIcon("resources/ui/icons/repeat-30.png");
	stretch_icon = QIcon("resources/ui/icons/stretch-30.png");
	save_icon = QIcon("resources/ui/icons/save-30.png");
}

void OutputTab::addComponent()
{
	top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(5, 5, 5, 5);

	header(OUTPUT_TAB_HEADER);
	body();

	QWidget* view_mode_widget = new QWidget();
	QHBoxLayout* view_mode_layout = new QHBoxLayout();
	view_mode_layout->setContentsMargins(0, 0, 0, 0);
	view_mode_layout->setSpacing(5);

	view_mode_widget->setLayout(view_mode_layout);

	add_pic(OUTPUT_TAB_DIFFUSE, "diffuse");
	add_pic(OUTPUT_TAB_ROUGHNESS, "roughness");
	add_pic(OUTPUT_TAB_METALLIC, "metallic");
	add_pic(OUTPUT_TAB_NORMAL_DISTURB, "normal_disturb");
	add_pic(OUTPUT_TAB_HEIGHT, "height");
	end_body();
	setLayout(top_layout);
}

void OutputTab::header(QString header)
{
	QWidget* head = new QWidget();

	UIModel* model = UIModel::get();
	head->setFixedHeight(model->control_selector_button_height);

	head->setObjectName("head");
	head->setStyleSheet("background-color:transparent;color:white;border:0px solid red;");

	QHBoxLayout* header_layout = new QHBoxLayout();
	header_layout->setContentsMargins(5, 0, 5, 0);
	header_layout->setSpacing(5);

	QPushButton* icon_btn = new QPushButton();
	icon_btn->setObjectName("header_icon");
	icon_btn->setStyleSheet("background-color:transparent;color:transparent;border:0px solid;");

	QIcon* icon = new QIcon("resources/ui/icons/output-30.png");

	icon_btn->setIcon(*icon);
	icon_btn->setIconSize(QSize(20, 20));
	icon_btn->setFixedSize(QSize(20, 20));
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
	int line_height = 120;
	
	ImgDrawer* pic_widget = new ImgDrawer();
	pic_widget->setStyleSheet("background-color:black;border-radius:4px;");
	imgs[tag] = pic_widget;

	QWidget* line_widget = new ScalableContainer(pic_widget);
	line_widget->setFixedHeight(line_height);

	ExpandableNode* preview_node = new ExpandableNode(IMG_DRAWER_PREVIEW, line_widget);

	QWidget* wrapper_widget = new QWidget();
	QVBoxLayout* wrapper_layout = new QVBoxLayout();
	wrapper_layout->setAlignment(Qt::AlignTop);
	wrapper_layout->setContentsMargins(0, 0, 0, 0);
	wrapper_layout->setSpacing(5);
	
	QWidget* operators_container = new QWidget();
	int operators_height = 20;
	QVBoxLayout* operators_layout = new QVBoxLayout();
	operators_layout->setContentsMargins(0, 0, 0, 0);

	QPushButton* view_mode_btn = new QPushButton();
	view_mode_btn->setObjectName("operator");
	view_mode_btn->setStyleSheet("QPushButton{background-color: #545454;}QPushButton:hover{background-color: #656565;}");
	view_mode_btn->setText(IMG_DRAWER_SWITCH_VIEW_MODE);
	view_mode_btn->setIcon(repeat_icon);
	view_mode_btn->setFixedHeight(operators_height);
	view_mode_btn->setMinimumHeight(operators_height);
	connect(view_mode_btn, &QPushButton::clicked, [=]() {
		ImgDrawer::Policy policy = pic_widget->switchPolicy();
		if (policy == ImgDrawer::Policy::Single) {
			view_mode_btn->setIcon(stretch_icon);
		}
		else if (policy == ImgDrawer::Policy::Repeat) {
			view_mode_btn->setIcon(repeat_icon);
		}
		pic_widget->update();
		});
	operators_layout->addWidget(view_mode_btn);

	QPushButton* save_btn = new QPushButton();
	save_btn->setObjectName("operator");
	save_btn->setStyleSheet("QPushButton{background-color: #545454;}QPushButton:hover{background-color: #656565;}");
	save_btn->setText(OUTPUT_TAB_SAVE);
	save_btn->setIcon(save_icon);
	save_btn->setFixedHeight(operators_height);
	connect(save_btn, &QPushButton::clicked, [=]() {
		QString f = QFileDialog::getSaveFileName(nullptr, "save", "", "*.png");
		if (f != "") {
			pic_widget->save_pic(f);
		}
		});
	operators_layout->addWidget(save_btn);

	operators_container->setLayout(operators_layout);

	wrapper_layout->addWidget(operators_container);

	wrapper_layout->addWidget(preview_node);

	wrapper_widget->setLayout(wrapper_layout);

	ExpandableNode* line_node = new ExpandableNode(title, wrapper_widget);
	
	body_layout->addWidget(line_node);
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
