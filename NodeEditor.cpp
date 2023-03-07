#include "NodeEditor.h"
NodeEditor::NodeEditor()
{
	init();
	addComponent();
}

NodePool* NodeEditor::get_pool()
{
	return pool;
}

void NodeEditor::init()
{
	
	offset_x = offset_y = 5;
	pool = new NodePool();
	setStyleSheet(CssLoader::load_css("node_editor.css"));

	widgets = vector<NodeWidget*>();
	curves = vector<NodeCurve*>();
}

void NodeEditor::addComponent()
{
	panel = new NodeEditorPanel();
	panel->responder = this;
	panel->setParent(this);
	panel->move(10, 10);
	panel->hide();
	
	/*
	NodeWidget* test_widget = new NodeWidget(this, 40, 40);
	test_widget->setParent(this);
	test_widget->move(40 - offset_x, 40 - offset_y);
	widgets.push_back(test_widget);

	NodeWidget* test_widget1 = new NodeWidget(this, 240, 140);
	test_widget1->setParent(this);
	test_widget1->move(240 - offset_x, 140 - offset_y);
	widgets.push_back(test_widget1);
	*/
}

void NodeEditor::btn_down(NodeWidget*)
{
}

void NodeEditor::open_panel()
{
}

void NodeEditor::close_panel()
{
}

void NodeEditor::add_dots()
{

}

void NodeEditor::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	QPen pen;
	QBrush brush;
	int dot_dist = 30;
	int big_dot_dist = dot_dist * 5;
	QColor dot_color(100, 100, 100);
	QColor curve_color(244, 244, 244);
	pen.setStyle(Qt::SolidLine);
	pen.setWidth(2);
	pen.setColor(dot_color);
	brush.setColor(dot_color);
	brush.setStyle(Qt::SolidPattern);

	painter.setPen(pen);
	painter.setBrush(brush);
	
	QRect rect = this->geometry();
	int width = rect.width();
	int height = rect.height();

	int x_start = ceilf(offset_x / dot_dist);
	int y_start = ceilf(offset_y / dot_dist);

	int standard_x = x_start * dot_dist - offset_x;

	int standard_y = y_start * dot_dist - offset_y;

	int y_count = y_start % 5;
	for (int start_y = standard_y; start_y < height; start_y += dot_dist) {
		int x_count = x_start % 5;
		for (int start_x = standard_x; start_x < width; start_x += dot_dist) {
			int width = 3;
			if (x_count == 0 && y_count == 0) {
				width = 5;
			}
			pen.setWidth(width);
			painter.setPen(pen);
			painter.drawPoint(start_x, start_y);
			x_count = (x_count + 1) % 5;
		}
		y_count = (y_count + 1) % 5;
	}
	QWidget::paintEvent(e);
	// 绘制曲线

	pen.setStyle(Qt::SolidLine);
	pen.setWidth(2);
	pen.setColor(curve_color);
	brush.setColor(curve_color);
	brush.setStyle(Qt::NoBrush);

	painter.setPen(pen);
	painter.setBrush(brush);

	int cl = curves.size();
	for (int i = 0; i < cl; i++) {
		NodeCurve* c = curves[i];
		QPoint start = mapFromGlobal(c->get_start_point());
		QPoint stop = mapFromGlobal(c->get_stop_point());
		painter.drawPath(curve_path(start,stop));
	}
	if (is_curving) {
		QPoint start = mapFromGlobal(temp_curve->get_start_point());
		QPoint stop = mapFromGlobal(QCursor::pos());
		painter.drawPath(curve_path(start, stop));
	}
}

void NodeEditor::mouseMoveEvent(QMouseEvent* e)
{
	//移动视框相关逻辑
	if (mouse_middle_down) {
		float current_x = e->pos().x();
		float current_y = e->pos().y();
		delta_mouse_x = current_x - prev_mouse_x;
		delta_mouse_y = current_y - prev_mouse_y;
		prev_mouse_x = current_x;
		prev_mouse_y = current_y;
		if (delta_mouse_x != 0) {
			offset_x -= delta_mouse_x;
		}
		if (delta_mouse_y != 0) {
			offset_y -= delta_mouse_y;
		}
		if (delta_mouse_x != 0 || delta_mouse_y != 0) {
			update();
			QPoint delta(delta_mouse_x, delta_mouse_y);
			update_widgets(delta);
		}
	}
}

void NodeEditor::mousePressEvent(QMouseEvent* e)
{
	prev_mouse_x = e->pos().x();
	prev_mouse_y = e->pos().y();
	mouse_down = true;
	if (e->button() == Qt::MiddleButton) {
		mouse_middle_down = true;
		if (panel && !panel->isHidden()) {
			panel->hide();
		}
	}
	else if (e->button() == Qt::RightButton) {
		if (panel) {
			if (panel->isHidden()) {
				panel->show();
				QPoint local = mapFromGlobal(e->globalPos());
				panel->move(local.x(), local.y());
			}
		}
	}
	else {
		if (panel && !panel->isHidden()) {
			panel->hide();
		}
	}
}

void NodeEditor::mouseReleaseEvent(QMouseEvent* e)
{
	mouse_down = false;
	if (e->button() == Qt::MiddleButton) {
		mouse_middle_down = false;
	}
	if (e->button() == Qt::LeftButton) {
		is_curving = false;
		temp_curve = nullptr;
		update();
	}
}

QPainterPath NodeEditor::curve_path(QPoint start, QPoint stop)
{
	QPainterPath path(start);

	int min_x = std::min(start.x(), stop.x());
	int min_y = std::min(start.y(), stop.y());
	int max_x = std::max(start.x(), stop.x());
	int max_y = std::max(start.y(), stop.y());

	QPoint topLeft(min_x, min_y);
	QPoint topRight(max_x, min_y);
	QPoint bottomLeft(min_x, max_y);
	QPoint bottomRight(max_x, max_y);

	if (start == topLeft) {
		QPoint c1(max_x, min_y);
		QPoint c2(min_x, max_y);
		path.cubicTo(c1, c2, stop);
	}
	else if (start == topRight) {
		QPoint c1(min_x, min_y);
		QPoint c2(max_x, max_y);
		path.cubicTo(c1, c2, stop);
	}
	else if (start == bottomRight) {
		QPoint c1(max_x, min_y);
		QPoint c2(min_x, max_y);
		path.cubicTo(c1, c2, stop);
	}
	else {
		QPoint c1(min_x, min_y);
		QPoint c2(max_x, max_y);
		path.cubicTo(c1, c2, stop);
	}
	return path;
}

void NodeEditor::update_widgets(QPoint delta)
{
	int s = widgets.size();
	for (int i = 0; i < s; i++) {
		QWidget* t = widgets[i];
		t->move(t->frameGeometry().topLeft() + delta);
	}
}

void NodeEditor::start_linking(NodeDataButton* button)
{
	is_curving = true;
	temp_curve = new NodeCurve();
	temp_curve->start = button;
}

void NodeEditor::release_on_me(QPoint pos)
{
	if (is_curving) {
		int wl = widgets.size();
		NodeDataButton* stop = nullptr;
		for (int i = 0; i < wl; i++) {
			NodeWidget* w = widgets[i];
			QPoint topLeft = w->mapToGlobal(QPoint(0, 0));
			QRect rect;
			rect.setTopLeft(topLeft);
			rect.setSize(w->size());
			if (rect.contains(pos)) {
				NodeDataButton* button = w->release_in_me(pos);
				if (button) {
					NodeDataButton* start = temp_curve->start;
					string start_value = pool->get_value_type(start->id);
					string stop_value = pool->get_value_type(button->id);
					if (start_value == stop_value && (start->is_adapter != button->is_adapter)) {
						temp_curve->stop = button;
						curves.push_back(temp_curve);
						if (start_value == "float") {
							if (start->is_adapter) {
								pool->connect<float>(button->id, start->id);
							}
							else {
								pool->connect<float>(start->id, button->id);
							}
						}
						else if (start_value == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >") {
							if (start->is_adapter) {
								pool->connect<string>(button->id, start->id);
							}
							else {
								pool->connect<string>(start->id, button->id);
							}
						}
					}
					break;
				}
			}
		}
	}
	is_curving = false;
	temp_curve = nullptr;
	update();
}

void NodeEditor::reset_curving()
{
	is_curving = false;
	temp_curve = nullptr;
	update();
}

void NodeEditor::on_widget_move()
{
	update();
}

void NodeEditor::update_temp_curve(QPoint pos)
{
	current_pos = pos;
	update();
}

void NodeEditor::btn_down(string tag)
{
	if (!panel) {
		return;
	}
	panel->hide();

	if (tag == "no") {
		QPoint ppos = panel->pos();
		NodeWidget* test_widget1 = new NodeWidget(this, ppos.x() + offset_x, ppos.y() + offset_y);
		test_widget1->setParent(this);
		test_widget1->move(ppos);
		widgets.push_back(test_widget1);
		test_widget1->show();
	} 

	else if (tag == "custom_layer") {
		QPoint ppos = panel->pos();
		CustomLayerNodeWidget* w = new CustomLayerNodeWidget(this, ppos.x() + offset_x, ppos.y() + offset_y);
		w->setParent(this);
		w->move(ppos);
		widgets.push_back(w);
		w->show();
	}
	else if (tag == "layer_machine") {
		QPoint ppos = panel->pos();
		LayerMachineNodeWidget* w = new LayerMachineNodeWidget(this, ppos.x() + offset_x, ppos.y() + offset_y);
		w->setParent(this);
		w->move(ppos);
		widgets.push_back(w);
		w->show();
	}
}


