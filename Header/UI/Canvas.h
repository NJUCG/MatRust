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
#include"StringHelper.h"

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
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
protected:
    vector<LightData*> lights_data;

    CanvasBackGround* background = nullptr;

    Model* model_itself = nullptr;
    Camera* camera = nullptr;
    vec3 init_loc;
    int scr_width, scr_height;
    bool setted = false;

    bool is_pipeline_on = false;
    float i_metallic = 0.0f;
    float i_roughness = 0.0f;

    unordered_map<string, ObjectData*> object_list;

    void init();
    void addComponent();
    void init_light();
    void set_up_light();


    
    // ��Ⱦģ��
    int metallic_index, roughness_index;

    void render_output();

    Shader* grid_shader;
    Shader* shader;

    GLuint VAO, VBO;
    void init_scene();
    void draw_scene();

    // ����ģ��
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

    float ortho_min = 0.1f;
    float perspect_min = 10.0f;

    float view_min = -1;

    float ortho_start = 1000.0f;
    float perspect_start = 5000.0f;

    float view_start = -1;

    bool is_ortho = true;

    float linear_zoom_controller = 3.0f;
    float direct_zoom_controller = 3.0f;

    // �����Ե�����ӳ�䵽�����Ե�zoom
    inline float zoom_project();

    void frame_consumer(float) override;

    // ��ģ������context���޸�
    string model_path = "";
    bool should_load = false;

    // �־û�����
    void load_config(QString);
    void save_config(QString);
};
