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
#include"EventAdapter.h"
#include"PipelineConfig.h"
#include"StringHelper.h"
using namespace std;

class EnvData {
public:
    float rh, cc, oc, sc, temperature;
    float metallic, roughness;
};

class EnvTab : public EventResponder, public QWidget, public FloatEditResponder
{
public:
    EnvTab();
    EnvTab(QWidget*);
    void on_trigger(string) override;
    void on_value_changed(string name, float new_value) override;
    void set_env(EnvData);
protected:
    QVBoxLayout* layout = nullptr;
    QVBoxLayout* body_layout = nullptr;
    QWidget* body_widget = nullptr;
    unordered_map<string, FloatEdit*> arguments;
    EnvData* backup_config = nullptr;

    int widget_id = 0;
    unordered_map<int, QWidget*> body_widgets;
    unordered_map<int, QVBoxLayout*> body_layouts;

    void init();
    void addComponent();
    void header(QString header);
    void body();
    void end_body();
    void argument(QString name, string tag, float init_value = 0.0f, float min = 0, float max = 100, float step = 0.1f,float sensitivity = 1.0f);
    void expand_panel(int widget_id = 0);
    void shrink_panel(int widget_id = 0);
    void space();

    // ³Ö¾Ã»¯
    void load_config(QString path);
    void save_config(QString path);
};

