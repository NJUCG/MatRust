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
#include<qcolordialog.h>
#include"AlgArgAnalyzer.h"
#include"StringHelper.h"
#include"NormalDisturbHelper.h"
#include"ImgDrawer.h"
#include"ExpandableNode.h"

// fov 调整
// 环境光
// 拉满参数以后生成大量铜锈
// 添加其他增长模式
// 
// mesh sampling
// 裁剪曲率
// 
// 高度图，法线扰动
// 自由截图

// 两套参数调节：物理环境，艺术设计
// 规律输入
// roughness 增加阈值
// 添加readme, 记得加<a target="_blank" href="https://icons8.com/icon/95809/plus-math">Plus Math</a> icon by <a target="_blank" href="https://icons8.com">Icons8</a>

// 随机数
// 4张图新窗口
// 整体UI布局
// 金属度和粗糙度弄成灰度图

int mainWindow(int argc,char* argv[]){
    QApplication a(argc, argv);
    QWidget* w = new QWidget();
    w->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored));

    w->setWindowTitle(APPLICATION_TITLE);
    w->setWindowIcon(QIcon("resources/ui/icons/icon-black-30.png"));

    PipelineManager::init_pipeline();
    _3DBlueNoiseSampler::init_samplers();

    w->setStyleSheet(CssLoader::load_css("our_father.css"));

    int spacing = UIModel::get()->common_spacing;

    // 绘制画布
    QVBoxLayout* top_layout = new QVBoxLayout();
    top_layout->setContentsMargins(0, 0, 0, 0);
    top_layout->setSpacing(spacing);

    Header* header = new Header();
    header->setFixedHeight(UIModel::get()->header_height);
    top_layout->addWidget(header);

    QLabel* blank_separator = new QLabel();
    blank_separator->setFixedHeight(4);
    blank_separator->setStyleSheet("background-color:black;");

    top_layout->addWidget(blank_separator);

    SceneTab* scene = new SceneTab();
    scene->setFixedHeight(250);

    ControllPanel* control = new ControllPanel();

    SeparatorContainer* container = new SeparatorContainer();
    container->setStyleSheet("background-color:black;");

    QHBoxLayout* container_layout = new QHBoxLayout();
    container_layout->setContentsMargins(0, 0, 0, 0);
    container_layout->setSpacing(spacing);

    SeparatorContainer* left_container = new SeparatorContainer();
    
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
    right_container->setFixedWidth(500);
    
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
    w->setWindowState(Qt::WindowMaximized);

    separator->init(left_container, right_container, true);

    player->register_timer(canvas);

    return a.exec();
}
int model_loading_test(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QWidget* w = new QWidget();
    PipelineManager::init_pipeline();
    _3DBlueNoiseSampler::init_samplers();
    w->setStyleSheet(CssLoader::load_css("our_father.css"));

    int spacing = UIModel::get()->common_spacing;

    // 绘制画布
    QVBoxLayout* top_layout = new QVBoxLayout();

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

    top_layout->addWidget(canvas_container);
    
    //model->release_model();
    //model->loadModel("D:/rust/CRust/QtWidgetsApplication1/QtWidgetsApplication1/resources/models/space_ship/space_ship.obj");
    //"resources/models/99 - intergalactic_spaceship - obj/Intergalactic_Spaceship-(Wavefront).obj"
    // "resources/models/plane/plane.obj"
    // "resources/models/stranger/stranger.obj"
    // "D:/rust/CRust/QtWidgetsApplication1/QtWidgetsApplication1/resources/models/stranger/stranger.obj"


    //player->register_timer(canvas);

    w->setLayout(top_layout);
    w->show();
    w->resize(800, 600);

    //Model* model = new Model();
    //model->loadModel("D:/rust/CRust/QtWidgetsApplication1/QtWidgetsApplication1/resources/models/stranger/stranger.obj");
    //canvas->setModel(model);

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
int palette_test(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QWidget* w = new QWidget();

    w->setStyleSheet(CssLoader::load_css("our_father.css"));

    QVBoxLayout* top_layout = new QVBoxLayout();
    top_layout->setContentsMargins(0, 0, 0, 0);
    top_layout->setSpacing(0);

    QPushButton* btn = new QPushButton();

    QColorDialog* dialog = new QColorDialog();

    top_layout->addWidget(btn);

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
int random_test() {
    int w, h;
    w = h = 400;
    int wavelength = 10;
    NormalDisturbHelper helper(w, h, wavelength);
    helper.generate_pattern("noise.png");
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

    QHBoxLayout* top_layout = new QHBoxLayout();

    QWidget* left = new QWidget();
    left->setStyleSheet("background-color:black;");
    top_layout->addWidget(left);

    QWidget* right = new QWidget();
    right->setStyleSheet("background-color:white");
    top_layout->addWidget(right);

    right->setFixedWidth(200);

    w->setLayout(top_layout);

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
int analyzer_test() {

    string test_str = "( sc + 2.1 ) * ( oc + 2.1 ) * ( rh + 0.1 ) ";
    PipelineConfig* config = new PipelineConfig();
    config->sc = 0.8f;
    config->oc = 1.0f;
    config->rh = 2.0f;

    AlgArgAnalyzer::analyzer->register_config(config);

    float t = AlgArgAnalyzer::analyzer->get_arg(test_str);
    cout << t << endl;
    return 0;
}
int strings_test() {
    StringHelper::init_string_helper();

    //cout << SCENE_TAB_HEADER_TITLE << endl;
    //cout << StringHelper::fetch("nihao_ch") << endl;

    return 0;
}
int img_drawer_test(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QWidget* w = new QWidget();
    w->setGeometry(0, 0, 600, 400);
    w->setStyleSheet("background-color:orange");
    QVBoxLayout* layout = new QVBoxLayout();

    int iw = 200;
    int ih = 200;
    vector<vector<vec4>> img(ih, vector<vec4>(iw, vec4()));
    for (int i = 0; i < ih; i++) {
        for (int j = 0; j < iw; j++) {
            img[i][j] = vec4(i, j, 100, 255);
        }
    }

    layout->addWidget(new ImgDrawer(200, 200, &img));

    w->setLayout(layout);

    w->show();
    return a.exec();
}
int expandable_node_test(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QWidget* w = new QWidget();
    w->setGeometry(0, 0, 600, 400);
    w->setStyleSheet("background-color:orange");
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);

    int iw = 200;
    int ih = 200;
    vector<vector<vec4>> img(ih, vector<vec4>(iw, vec4()));
    for (int i = 0; i < ih; i++) {
        for (int j = 0; j < iw; j++) {
            img[i][j] = vec4(i, j, 100, 255);
        }
    }

    QWidget* drawer = new ImgDrawer(200, 200, &img);

    QLabel* la = new QLabel("noew");
    la->setFixedHeight(50);

    ExpandableNode* node = new ExpandableNode("Test", drawer);
    
    layout->addWidget(node);

    QWidget* drawer2 = new ImgDrawer(200, 200, &img);
    drawer2->setFixedHeight(200);

    QLabel* la2 = new QLabel("noesdafw");
    la2->setFixedHeight(50);

    ExpandableNode* node2 = new ExpandableNode("Test2", la2);

    layout->addWidget(node2);

    layout->addWidget(new QWidget());

    w->setLayout(layout);

    w->show();
    return a.exec();
}
int animation_test(int argc, char* argv[]) {
    QApplication a(argc, argv);
    QWidget* w = new QWidget();
    w->setGeometry(0, 0, 600, 400);
    w->setStyleSheet("background-color:orange");
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);

    int iw = 200;
    int ih = 200;
    vector<vector<vec4>> img(ih, vector<vec4>(iw, vec4()));
    for (int i = 0; i < ih; i++) {
        for (int j = 0; j < iw; j++) {
            img[i][j] = vec4(i, j, 100, 255);
        }
    }
    
    QPropertyAnimation* animator = new QPropertyAnimation();
    QWidget* c = new QWidget();
    QVBoxLayout* lay = new QVBoxLayout();
    c->setLayout(lay);

    QLabel* la = new QLabel("noew");
    la->setStyleSheet("background-color:black;color:white;");
    animator->setTargetObject(la);
    animator->setPropertyName("geometry");
    animator->setEasingCurve(QEasingCurve::Linear);

    lay->addWidget(la);
    layout->addWidget(c);

    QPushButton* btn = new QPushButton();
    QObject::connect(btn, &QPushButton::clicked, [animator]() {
        animator->setDuration(1000);
        animator->setStartValue(QRect());
        animator->setEndValue(QRect(0,0,100,200));
        animator->start();
        });
    layout->addWidget(btn);
    layout->addWidget(new QWidget());

    w->setLayout(layout);

    w->show();
    return a.exec();
}
//
// 材质rounding
// demo
// github结构
// 整体加载
//
int main(int argc, char *argv[]){
    EventAdapter::init_event_adapter();
    AlgArgAnalyzer::init_analyzer();
    StringHelper::init_string_helper();
    //return analyzer_test();
    return mainWindow(argc, argv);
    //return strings_test();
    //return random_test();
    //return expandable_node_test(argc, argv);
    //return animation_test(argc, argv);
    //return img_drawer_test(argc, argv);
    //return layout_test(argc, argv);
}
