#include <QtWidgets/QApplication>
#include <QtOpenGL/qopenglwindow.h>
#include <QtOpenGLWidgets/qopenglwidget.h>
#include "Canvas.h"
#include <QLabel>
#include "CssLoader.h"
#include "ControllerSelector.h"
#include "ControllPanel.h"
#include "TabEntry.h"
#include <qtextcodec.h>
#include<qdebug.h>
#include"NodeEditor.h"
#include"PlayerWidget.h"
#include"Header.h"
#include"Separator.h"
#include"SeparatorContainer.h"
#include "PipelineManager.h"
#include"ImageHelper.h"
#include"SceneTab.h"
#include"EventAdapter.h"

// fov 调整
// 环境光
// 拉满参数以后生成大量铜锈
// 添加其他增长模式
// 
// mesh sampling
// 裁剪曲率
// 
// 高度图
// 自由截图

// 两套参数调节：物理环境，艺术设计
// 规律输入

int mainWindow(int argc,char* argv[]){
    QApplication a(argc, argv);
    QWidget* w = new QWidget();
    PipelineManager::init_pipeline();
    
    w->setStyleSheet(CssLoader::load_css("our_father.css"));

    int spacing = UIModel::get()->common_spacing;

    // 绘制画布
    QVBoxLayout* top_layout = new QVBoxLayout();
    Header* header = new Header();
    header->setFixedHeight(UIModel::get()->header_height);

    SceneTab* scene = new SceneTab();
    ControllPanel* control = new ControllPanel();

    top_layout->addWidget(header);
    top_layout->setContentsMargins(0, 0, 0, 0);
    top_layout->setSpacing(spacing);

    SeparatorContainer* container = new SeparatorContainer();
    container->setStyleSheet("background-color:black;");

    QHBoxLayout* container_layout = new QHBoxLayout();
    container_layout->setContentsMargins(0, 0, 0, 0);
    container_layout->setSpacing(spacing);

    SeparatorContainer* left_container = new SeparatorContainer();
    //left_container->setStyleSheet("background-color:gray;border:2px solid blue;");
    
    QVBoxLayout* left_container_layout = new QVBoxLayout();
    
    left_container_layout->setContentsMargins(0, 0, 0, 0);
    left_container_layout->setSpacing(spacing);
    
    QWidget* canvas_container = new QWidget();
    canvas_container->setStyleSheet("border-radius:4px");

    QVBoxLayout* canvas_container_layout = new QVBoxLayout();
    canvas_container_layout->setContentsMargins(0, 0, 0, 0);

    Canvas* canvas = new Canvas();
    canvas->app = &a;
    canvas->wid = w->winId();
    canvas->father = w;
    canvas_container_layout->addWidget(canvas);

    canvas_container->setLayout(canvas_container_layout);

    left_container_layout->addWidget(canvas_container);
    left_container->addContainer(canvas_container);

    Separator* left_separator = new Separator();

    left_container_layout->addWidget(left_separator);

    PlayerWidget* player = new PlayerWidget();
    left_container_layout->addWidget(player);
    left_container->addContainer(player, player->maximumSize());
    
    left_container->setLayout(left_container_layout);
    left_separator->init(canvas_container, player, false);

    container->addContainer(left_container);

    container_layout->addWidget(left_container);

    Separator* separator = new Separator();

    container_layout->addWidget(separator);

    QWidget* right_container = new QWidget();
    
    QVBoxLayout* right_container_layout = new QVBoxLayout();
    
    right_container_layout->setContentsMargins(0, 0, 0, 0);
    right_container_layout->setSpacing(spacing);
    
    Separator* right_separator = new Separator();
    right_container_layout->addWidget(scene);

    right_container_layout->addWidget(right_separator);

    right_container_layout->addWidget(control);

    right_container->setLayout(right_container_layout);
    right_separator->init(scene, control, false);
    container->addContainer(right_container);

    container_layout->addWidget(right_container);

    container->setLayout(container_layout);
    
    top_layout->addWidget(container);
    
    w->setLayout(top_layout);
    w->show();
    w->resize(800, 600);

    separator->init(left_container, right_container, true);

    Model* model = new Model("resources/models/space_ship/space_ship.obj");
    //"resources/models/99 - intergalactic_spaceship - obj/Intergalactic_Spaceship-(Wavefront).obj"
    canvas->setModel(model);

    player->register_timer(canvas);

    return a.exec();
}
int test_window(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QWidget* w = new QWidget();

    w->setStyleSheet(CssLoader::load_css("our_father.css"));

    QVBoxLayout* top_layout = new QVBoxLayout();
    top_layout->setContentsMargins(0, 0, 0, 0);
    top_layout->setSpacing(0);
    
    SceneTab* tab = new SceneTab();
    tab->add_property("mesh-30", "hello");
    tab->add_property("light-30", "hello2");
    top_layout->addWidget(tab);

    w->setLayout(top_layout);
    w->show();
    w->resize(800, 600);

    return a.exec();
}
int cssLoader_test() {
    cout << CssLoader::load_css("controller_panel_style.css").toStdString() << endl;
    string t = CssLoader::load_css("controller_panel_style.css").toStdString();
    return 0;
}
int entry_test(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QWidget* w = new QWidget();
    w->setGeometry(0, 0, 600, 400);
    w->setStyleSheet("background-color:red");
    QVBoxLayout* outer_layout = new QVBoxLayout();

    FloatEdit* editor = new FloatEdit();
    editor->setFixedHeight(UIModel::get()->float_edit_height);
    outer_layout->addWidget(editor);
    
    w->setLayout(outer_layout);
    w->show();
    return a.exec();
}
int layout_test(int argc, char* argv[]) 
{
    QApplication a(argc, argv);

    QWidget* w = new QWidget();
    w->setGeometry(0, 0, 600, 400);
    w->setStyleSheet("background-color:red");

    QVBoxLayout* outer_layout = new QVBoxLayout();

    QWidget* container = new QWidget();
    container->setStyleSheet("background-color:black");
    outer_layout->addWidget(container);

    w->setLayout(outer_layout);
    
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(10);
    for (int i = 0; i < 6; i++) {
        QHBoxLayout* hlayout = new QHBoxLayout();

        QWidget* w = new QWidget();
        w->setStyleSheet("QWidget{color:white;background-color:grey}QLabel{background-color:black}");

        hlayout->addStretch(3);

        QLabel* t = new QLabel((string("hello ") + to_string(i)).c_str());
      
        t->setParent(w);
        hlayout->addWidget(t, 7);

        w->setLayout(hlayout);
        layout->addWidget(w, Qt::AlignRight);
        
    }
    container->setLayout(layout);

    w->show();
    return a.exec();
}
int node_test() {
    NodePool* pool = new NodePool();
    int source = pool->creat<float>();
    int data0 = pool->creat<float>();
    int data1 = pool->creat<float>();
    int data2 = pool->creat<float>();

    pool->connect<float>(source, data0);
    pool->connect<float>(data0, data1);
    pool->connect<float>(data1, data2);

    NodeData<float>* source_data = pool->get_data<float>(source);
    NodeData<float>* data0_data = pool->get_data<float>(data0);
    NodeData<float>* data1_data = pool->get_data<float>(data1);
    NodeData<float>* data2_data= pool->get_data<float>(data2);
    pool->set_value<float>(source, 5.7f);
    pool->set_value<float>(source, 5.8f);
    pool->set_value<float>(data0, 5.76f);
    pool->cut_off<float>(source, data0);
    pool->set_value(data0, 5.78f);
    pool->connect<float>(source, data0);

    return 0;
}
int node_editor_test(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QWidget* w = new QWidget();
    w->setGeometry(0, 0, 600, 400);
    w->setStyleSheet("background-color:orange");
    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(new NodeEditor());
    w->setLayout(layout);

    w->show();
    return a.exec();
}
int node_test(int argc, char* argv[]) {
    QApplication a(argc, argv);

    QWidget* w = new QWidget();
    w->setGeometry(0, 0, 600, 400);
    w->setStyleSheet("background-color:orange");
    NodeDataButton* button = new NodeDataButton(NULL, 0, true, "orange");
    button->setParent(w);
    button->move(150, 150);

    //QVBoxLayout* layout = new QVBoxLayout();
    //FloatSliderBar* bar = new FloatSliderBar(new FloatSliderBarResponder, "泰斯特", 0, 100);
    //layout->addWidget(new NodeWidget(new NodeWidgetResponder()));
    //layout->addWidget(bar);
    //int k = bar->value();
    //layout->addWidget(button);

    //w->setLayout(layout);

    w->show();
    return a.exec();
}
int player_test(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QWidget* w = new QWidget();
    w->setGeometry(0, 0, 600, 400);
    w->setStyleSheet("background-color:orange");
    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(new PlayerWidget());
    layout->addWidget(new QWidget());

    w->setLayout(layout);

    w->show();
    return a.exec();
}
int image_test() {
    int w, h;
    w = h = 256;
    unsigned char* data = new unsigned char[256 * 256 * 3];
    int idx = 0;
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            data[idx++] = 100;
            data[idx++] = 200;
            data[idx++] = 100;
        }
    }
    ImageHelper::save_pic("hello.png", data, 3, w, h);
    return 0;
}
int main(int argc, char *argv[]){
    EventAdapter::init_event_adapter();
    return mainWindow(argc, argv);
}
