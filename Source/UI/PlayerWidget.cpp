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
	//setFixedHeight(100);
	setStyleSheet(CssLoader::load_css("player_widget.css"));
	play_icon = QIcon("resources/ui/icons/play-30.png");
	pause_icon = QIcon("resources/ui/icons/pause-30.png");
}

void PlayerWidget::addComponent()
{
	setFixedHeight(60);

	top_layout = new QVBoxLayout();
	top_layout->setContentsMargins(5, 5, 5, 5);
	top_layout->setSpacing(0);

	header();
	body();

	setLayout(top_layout);
}

void PlayerWidget::header()
{
	QWidget* header_widget = new QWidget();
	header_layout = new QHBoxLayout();
	header_layout->setSpacing(0);
	header_layout->setContentsMargins(0, 0, 0, 0);
	header_widget->setFixedHeight(20);

	QWidget* icon_widget = new QWidget();

	QHBoxLayout* icon_layout = new QHBoxLayout();
	icon_layout->setContentsMargins(5, 0, 5, 0);
	icon_layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	QPushButton* icon_btn = new QPushButton();
	icon_btn->setIcon(QIcon("resources/ui/icons/time-30.png"));
	icon_btn->setEnabled(false);
	icon_btn->setIconSize(QSize(20, 20));
	icon_btn->setFixedSize(20, 20);
	
	icon_layout->addWidget(icon_btn);

	QLabel* tab_label = new QLabel(PLAYER_TITLE);
	tab_label->setStyleSheet("color:white;");

	icon_layout->addWidget(tab_label);

	icon_widget->setLayout(icon_layout);

	header_layout->addWidget(icon_widget);

	player();
	//ÓÒ²à¿É¶¯¿Õ¼ä
	header_layout->addWidget(new QWidget());

	header_layout->setStretch(0, 1); 
	header_layout->setStretch(1, 1);
	header_layout->setStretch(2, 1);

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
	QWidget* operators_widget = new QWidget();
	QHBoxLayout* operators_layout = new QHBoxLayout();
	operators_layout->setContentsMargins(0, 0, 0, 0);
	operators_layout->setAlignment(Qt::AlignCenter);
	operators_layout->setSpacing(1);

	play_and_pause_button = new QPushButton();
	play_and_pause_button->setObjectName("play_and_pause");
	play_and_pause_button->setIcon(play_icon);
	play_and_pause_button->setIconSize(QSize(20, 20));
	play_and_pause_button->setFixedSize(20, 20);
	play_and_pause_button->setCheckable(true);
	operators_layout->addWidget(play_and_pause_button);

	connect(play_and_pause_button, &QPushButton::clicked, [this]()->void {
		if (play_and_pause_button->isChecked()) {
			time_machine->play();
			play_and_pause_button->setIcon(pause_icon);
		}
		else {
			time_machine->pause();
			play_and_pause_button->setIcon(play_icon);
		}
	});

	replay_button = new QPushButton();
	replay_button->setObjectName("replay");
	replay_button->setFixedSize(20, 20);
	replay_button->setIconSize(QSize(20, 20));
	replay_button->setIcon(QIcon("resources/ui/icons/replay-30.png"));
	operators_layout->addWidget(replay_button);

	connect(replay_button, &QPushButton::clicked, [this]()->void {
		time_machine->clear();
		if (play_and_pause_button->isChecked()) {
			play_and_pause_button->setChecked(false);
		}
		});

	operators_widget->setLayout(operators_layout);

	header_layout->addWidget(operators_widget);
}


void PlayerWidget::on_time_exhausted()
{
	play_and_pause_button->setChecked(false);
}
