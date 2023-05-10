#include "ExpandableNode.h"

ExpandableNode::ExpandableNode()
{
	init();
	addComponent();
}

ExpandableNode::ExpandableNode(QString title, QWidget* content)
{
	this->title = new QLabel(title);
	this->content = content;
	container_height = content->height();

	init();
	addComponent();
}

void ExpandableNode::set_title(QString title)
{
	if (!this->title) {
		this->title = new QLabel(title);
	}
	else {
		this->title->setText(title);
	}
}

void ExpandableNode::set_content(QWidget* content)
{
	container_layout->removeWidget(container_layout->itemAt(0)->widget());
	delete (this->content);
	this->content = content;
	container_height = content->height();
	container_layout->addWidget(this->content);
}

void ExpandableNode::expand()
{
	if (expanded) {
		return;
	}
	container->show();
	animator->setPropertyName("geometry");
	animator->setDuration(100); 
	animator->setStartValue(QRect(0, title_height, width(), 0));
	animator->setEndValue(QRect(0, title_height, width(), container_height + 10));
	animator->start();
}

void ExpandableNode::shrink()
{
	if (!expanded) {
		return;
	}
	container_height = container->height();
	animator->setPropertyName("geometry");
	animator->setDuration(100);
	animator->setStartValue(QRect(0, title_height, width(), container_height + 10));
	animator->setEndValue(QRect(0, title_height, width(), 0));
	animator->start();
}

void ExpandableNode::init()
{
	setStyleSheet(CssLoader::load_css("expandable_node_style.css"));
	right_icon = new QIcon("resources/ui/icons/right_arrow_30.png");
	down_icon = new QIcon("resources/ui/icons/down_arrow_30.png");
}

void ExpandableNode::addComponent()
{
	top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(0);

	QSizePolicy policy = sizePolicy();
	policy.setVerticalPolicy(QSizePolicy::Policy::Maximum);
	setSizePolicy(policy);

	title_container = new QWidget();
	title_container->setFixedHeight(title_height);
	
	QHBoxLayout* title_layout = new QHBoxLayout();
	title_layout->setAlignment(Qt::AlignLeft);
	title_layout->setContentsMargins(5, 0, 5, 0);
	title_layout->setSpacing(5);
	
	QPushButton* expander = new QPushButton();
	expander->setObjectName("expander");

	expander->setIcon(*right_icon);
	expander->setIconSize(QSize(title_height, title_height));
	expander->setFixedSize(title_height, title_height);

	connect(expander, &QPushButton::clicked, [=]() {
		if (!expanded) {
			expand();
			expander->setIcon(*down_icon);
		}
		else {
			shrink();
			expander->setIcon(*right_icon);
		}
		});
	title_layout->addWidget(expander);

	if (!title) {
		title = new QLabel("Default");
	}
	title_layout->addWidget(title);
	
	title_container->setLayout(title_layout);

	title_container->setLayout(title_layout);

	top_layout->addWidget(title_container);
	
	container = new QWidget();
	container->setObjectName("container");

	animator = new QPropertyAnimation(container, "geometry", this);
	connect(animator, &QPropertyAnimation::finished, [=]() {
		if (expanded) {
			container->hide();
		}
		expanded = !expanded;
		});
	animator->setTargetObject(container);
	animator->setEasingCurve(QEasingCurve::Linear);
	
	container_layout = new QVBoxLayout();
	container_layout->setAlignment(Qt::AlignTop);
	container_layout->setContentsMargins(15, 0, 15, 5);
	container_layout->setSpacing(5);
	
	if (content) {
		container_layout->addWidget(content);
		container_height = content->height();
	}
	
	container->setLayout(container_layout);

	top_layout->addWidget(container);
	
	setLayout(top_layout);

	shrink();
}
