#pragma once
#include <qwidget.h>
#include<qlayout.h>
#include"CssLoader.h"
#include"UIModel.h"
#include<qpushbutton.h>
#include<qlabel.h>
#include<qtextcodec.h>
#include<unordered_map>
#include"FloatEdit.h"
using namespace std;
class EnvTab :
    public QWidget
{
public:
    EnvTab();
    EnvTab(QWidget*);
protected:
    QVBoxLayout* layout = nullptr;
    QVBoxLayout* body_layout = nullptr;
    QWidget* body_widget = nullptr;
    unordered_map<string, FloatEdit*> arguments;

    int widget_id = 0;
    unordered_map<int, QWidget*> body_widgets;
    unordered_map<int, QVBoxLayout*> body_layouts;

    void init();
    void addComponent();
    void header(string header);
    void body();
    void end_body();
    void argument(string name, float min = 0, float max = 100, float step = 0.1f,float sensitivity = 1.0f);
    void expand_panel(int widget_id = 0);
    void shrink_panel(int widget_id = 0);
    void space();

};

