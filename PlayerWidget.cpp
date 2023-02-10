#include "PlayerWidget.h"

PlayerWidget::PlayerWidget()
{
	init();
	addComponent();
}

PlayerWidget::~PlayerWidget()
{
	if (top_layout) {
		delete top_layout;
	}
	if (header_layout) {
		delete header_layout;
	}
}

void PlayerWidget::register_timer(TimeMachineResponder* r)
{
	time_machine->register_responder(r);
}

void PlayerWidget::init()
{
	setFixedHeight(100);
	setStyleSheet(CssLoader::load_css("player_widget.css"));
}

void PlayerWidget::addComponent()
{
	top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(0, 0, 0, 0);
	top_layout->setSpacing(0);

	header();
	body();

	setLayout(top_layout);
}

void PlayerWidget::header()
{
	QWidget* header_widget = new QWidget();
	header_layout = new QHBoxLayout();
	header_layout->setSpacing(2);
	header_layout->setContentsMargins(0, 0, 0, 0);
	header_widget->setFixedHeight(40);

	//左侧可动空间
	header_layout->addWidget(new QWidget());
	player();
	//右侧可动空间
	header_layout->addWidget(new QWidget());

	header_widget->setLayout(header_layout);
	top_layout->addWidget(header_widget);
}

void PlayerWidget::body()
{
	QWidget* body = new QWidget();
	body_layout = new QVBoxLayout();


	time_machine = new TimeMachineIdentifier();
	body_layout->addWidget(time_machine);

	connect(time_machine, &TimeMachineIdentifier::time_exhausted, this, &PlayerWidget::on_time_exhausted);

	body->setLayout(body_layout);
	top_layout->addWidget(body);
}

void PlayerWidget::player()
{
	play_and_pause_button = new QPushButton();
	play_and_pause_button->setObjectName("play_and_pause");
	play_and_pause_button->setFixedSize(30, 30);
	play_and_pause_button->setCheckable(true);
	header_layout->addWidget(play_and_pause_button);

	connect(play_and_pause_button, &QPushButton::clicked, [this]()->void {
		if (play_and_pause_button->isChecked()) {
			time_machine->play();
		}
		else {
			time_machine->pause();
		}
	});

	replay_button = new QPushButton();
	replay_button->setObjectName("replay");
	replay_button->setFixedSize(30, 30);
	header_layout->addWidget(replay_button);

	connect(replay_button, &QPushButton::clicked, [this]()->void {
		time_machine->clear();
		if (play_and_pause_button->isChecked()) {
			play_and_pause_button->setChecked(false);
		}
		});
}


void PlayerWidget::on_time_exhausted()
{
	play_and_pause_button->setChecked(false);
}
