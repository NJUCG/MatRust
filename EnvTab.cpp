#include "EnvTab.h"

EnvTab::EnvTab()
{
	init();
	addComponent();
}

EnvTab::EnvTab(QWidget* parent): QWidget(parent)
{
	init();
	addComponent();
}

void EnvTab::init()
{
	arguments = unordered_map<string, FloatEdit*>();
	body_widgets = unordered_map<int, QWidget*>();
	setStyleSheet(CssLoader::load_css("env_tab.css"));
}

void EnvTab::addComponent()
{
	layout = new QVBoxLayout();
	
	layout->setContentsMargins(5, 5, 5, 5);
	// 设置表头
	header("环境参数");
	body();
	
	argument(string("相对湿度"), 0, 100, 0.1f);
	argument(string("碳浓度"), 0, 100, 0.1f);
	argument(string("硫浓度"), 0, 100, 0.1f);
	argument(string("氧气浓度"), 0, 100, 0.1f);
	argument(string("温度"), 273, 373, 0.1f);
	argument(string("粗糙度"), 0, 1, 0.01f, 0.01f);
	argument(string("金属度"), 0, 1, 0.01f, 0.01f);
	end_body();

	header("wht");
	body();

	argument(string("相对湿度"), 0, 100, 0.1f);
	argument(string("碳浓度"), 0, 100, 0.1f);
	argument(string("硫浓度"), 0, 100, 0.1f);
	argument(string("氧气浓度"), 0, 100, 0.1f);
	argument(string("温度"), 273, 373, 0.1f);
	space();
	argument(string("粗糙度"), 0, 1, 0.01f, 0.01f);
	argument(string("金属度"), 0, 1, 0.01f, 0.01f);
	end_body();

	layout->addWidget(new QWidget());

	
	setLayout(layout);
}

void EnvTab::header(string header)
{
	QWidget* head = new QWidget();

	UIModel* model = UIModel::get();
	head->setFixedHeight(model->control_panel_header_height);

	head->setObjectName("head");

	QHBoxLayout* header_layout = new QHBoxLayout();
	header_layout->setContentsMargins(0,0,0,0);

	QPushButton* header_icon = new QPushButton();
	header_icon->setObjectName("header_icon");
	header_icon->setFixedWidth(model->control_panel_header_icon_width);
	header_icon->setCheckable(true);
	header_icon->setChecked(true);
	int cur_widget = widget_id;
	connect(header_icon, &QPushButton::clicked, [this, header_icon, cur_widget]()->void {
		if (!header_icon->isChecked()) {
			shrink_panel(cur_widget);
		}
		else {
			expand_panel(cur_widget);
		}
		});
	
	header_layout->addWidget(header_icon); 


	QLabel* value_label = new QLabel();
	value_label->setText(QString::fromUtf8(header.c_str()));
	value_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	header_layout->addWidget(value_label);

	header_layout->addWidget(new QWidget());

	head->setLayout(header_layout);
	
	
	layout->addWidget(head);
}

void EnvTab::body()
{
	body_widget = new QWidget();
	body_layout = new QVBoxLayout();
	body_layout->setContentsMargins(0, 0, 0, 0);
	
	body_widget->setLayout(body_layout);
	layout->addWidget(body_widget);
}

void EnvTab::end_body()
{
	body_layout->addWidget(new QWidget());
	
	body_widgets[widget_id] = body_widget;
	body_layouts[widget_id] = body_layout;

	widget_id++;
}

void EnvTab::argument(string name, float min, float max, float step, float sensitivity)
{
	QWidget* line = new QWidget();
	int line_height = UIModel::get()->control_panel_line_height;
	line->setFixedHeight(line_height);

	QHBoxLayout* line_layout = new QHBoxLayout();
	line_layout->setContentsMargins(0, 0, 0, 0);
	line_layout->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

	
	line_layout->addWidget(new QWidget());
	
	QLabel* value_label = new QLabel();
	value_label->setText(QString::fromUtf8(name.c_str()));
	value_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	line_layout->addWidget(value_label);
	
	FloatEdit* editor = new FloatEdit();
	editor->set_min_value(min);
	editor->set_max_value(max);
	editor->step = step;
	editor->sensitivity = sensitivity;
	arguments[name] = editor;
	editor->setFixedWidth(UIModel::get()->control_panel_edit_width);
	line_layout->addWidget(editor);
	line->setLayout(line_layout);
	
	body_layout->addWidget(line);
}

void EnvTab::expand_panel(int widget_id)
{
	body_widgets[widget_id]->show();
}

void EnvTab::shrink_panel(int widget_id)
{
	body_widgets[widget_id]->hide();
}

void EnvTab::space()
{
	QWidget* space = new QWidget();
	int line_height = UIModel::get()->control_panel_line_height;
	space->setFixedHeight(line_height);
	body_layout->addWidget(space);
}


