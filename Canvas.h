#pragma once
#include<QtOpenGLWidgets/qopenglwidget.h>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>
#include"CssLoader.h"
#include"Model.h"
#include"Camera.h"
#include"UIModel.h"
#include<qevent.h>
#include<algorithm>
#include"CanvasBackGround.h"
#include"PipelineManager.h"
#include"LightData.h"
#include"EventAdapter.h"
#include<qpushbutton.h>
#include<qimage.h>
#include<qapplication.h>

#define CANVAS_VIEW_NONE 0
#define CANVAS_VIEW_ROTATE 0x1
#define CANVAS_VIEW_MOVE 0x3

using namespace std;


class Canvas : public QOpenGLWidget, public TimeMachineResponder, public EventResponder
{
    Q_OBJECT
public:
    Canvas();
    ~Canvas();
    void setModel(Model*);
    void on_trigger(string) override;
    void time_up() override;
    QApplication* app = nullptr;
    WId wid;
    QWidget* father;
protected:
    vector<LightData*> lights_data;

    CanvasBackGround* background = nullptr;

    Model* model_itself = nullptr;
    Camera* camera = nullptr;
    vec3 init_loc;
    int scr_width, scr_height;

    unordered_map<string, ObjectData*> object_list;

    void init();
    void addComponent();
    void init_light();
    void set_up_light();

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w,int h) override;
    
    // 渲染模块
    int metallic_index, roughness_index;

    void render_output();

    Shader* grid_shader;
    Shader* shader;

    GLuint VAO, VBO;
    void initTriangle();
    void drawTriangle();


    
    // 交互模块
    unsigned int view_state = 0;
    QPoint prev_cursor;
    float move_sensitivity;
    float rotate_sensitivity;

    void mousePressEvent(QMouseEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
    void keyReleaseEvent(QKeyEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;

    float ortho_min = 0.4f;
    float perspect_min = 10.0f;

    float view_min = -1;

    float ortho_start = 1000.0f;
    float perspect_start = 5000.0f;

    float view_start = -1;

    bool is_ortho = false;

    float linear_zoom_controller = 3.0f;
    float direct_zoom_controller = 3.0f;

    // 将线性的轮椅映射到非线性的zoom
    inline float zoom_project();

    void frame_consumer(float) override;
};
