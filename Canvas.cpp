#include "Canvas.h"

Canvas::Canvas() {
    init();
    addComponent();
}
Canvas::~Canvas() {

}
void Canvas::setModel(Model* model)
{
    model_itself = model;
}

void Canvas::on_trigger(string name)
{
    if (name == string("object_value_changed")) {
        ObjectData* obj = (ObjectData*)(EventAdapter::shared->pop_data());
        if (obj) {
            if (obj->type == "mesh") {
                object_list[obj->tag] = obj;
            }
            else if (obj->type == "light") {
                lights_data[obj->idx] = (LightData*)obj;
            }
            update();
        }
    }
    else if (name == "selected_mesh_changed") {
        QString* path = (QString*)(EventAdapter::shared->pop_data());
        if (path) {
            model_path = path->toStdString();
            should_load = true;
            update();
        }
    }
    else if (name == "combine_config_event_canvas") {
        PipelineConfig* c = (PipelineConfig*)EventAdapter::shared->pop_data();
        
        c->backTexture = vector<vector<vec4>>(c->textureHeight, vector<vec4>(c->textureWidth, vec4(255, 80, 60, 255)));
        c->curvature = CurvatureHelper::generate("resources/models/stranger/curve.png");

        is_pipeline_on = true;

        EventAdapter::shared->push_data(c);
    }
    else if (name == "model_back_roughness_changed") {
        float r = *((float*)EventAdapter::shared->pop_data());
        i_roughness = r;
    }
    else if (name == "model_back_metallic_changed") {
        float m = *((float*)EventAdapter::shared->pop_data());
        i_metallic = m;
    }
}

void Canvas::time_up()
{
    QScreen* scr = QGuiApplication::primaryScreen();
    scr->grabWindow(father->winId(), 0, UIModel::get()->header_height, width(), height()).save("123.jpg", "jpg");
}

void Canvas::init()
{
    if (!model_itself) {
        model_itself = new Model();
    }
    lights_data = vector<LightData*>();
    setStyleSheet("border-radius:4px;");
    camera = new Camera();
    UIModel* m = UIModel::get();

    this->init_loc = m->init_loc;

    ObjectData* obj = new ObjectData();
    obj->name = SCENE_TAB_OBJECT_TITLE.toStdString();
    obj->tag = "object";
    obj->type = "mesh";
    obj->loc = vec4(init_loc.x, init_loc.y, init_loc.z, 1);
    obj->rot = vec4(0, 0, 0, 0);
    obj->scl = vec4(1, 1, 1, 1);
    EventAdapter::shared->push_data(obj);
    EventAdapter::shared->trigger_event("add_object_to_tab");
    EventAdapter::shared->push_data(obj);
    EventAdapter::shared->trigger_event("selected_object_changed");
    object_list[obj->name] = obj;

    this->scr_width = m->src_width;
    this->scr_height = m->src_height;

    this->move_sensitivity = m->canvas_view_move_sensitivity;
    this->rotate_sensitivity = m->canvas_view_rotate_sensitivity;

    this->metallic_index = m->metallic_index;
    this->roughness_index = m->roughness_index;

    if (is_ortho) {
        view_start = ortho_start;
        view_min = ortho_min;
    }
    else {
        view_start = perspect_start;
        view_min = perspect_min;
    }

    this->linear_zoom_controller = view_start;
    this->direct_zoom_controller = zoom_project();
    this->object_list = unordered_map<string, ObjectData*>();

    EventAdapter::shared->register_event("object_value_changed", this);
    EventAdapter::shared->register_event("selected_mesh_changed", this);
    EventAdapter::shared->register_event("combine_config_event_canvas", this);

    EventAdapter::shared->register_event("model_back_roughness_changed", this);
    EventAdapter::shared->register_event("model_back_metallic_changed", this);

    init_light();

}
void Canvas::addComponent()
{

}
void Canvas::init_light()
{
    int t = 16;
    vec3 lightIntensity(t * 6, t * 6, t * 6);
    vec3 lightPos(0, init_loc.y + 6, init_loc.z + 3);
    LightData* lght = new LightData();
    lght->name = SCENE_TAB_LIGHT_TITLE_0.toStdString();
    lght->light_name = "lightPosition[0]";
    lght->loc = vec4(lightPos.x, lightPos.y, lightPos.z, 1);
    lght->rot = vec4(0, 0, 0, 0);
    lght->scl = vec4(1, 1, 1, 1);
    lght->light_intensity = lightIntensity;
    lght->type = "light";
    lght->idx = 0;
    EventAdapter::shared->push_data(lght);
    EventAdapter::shared->trigger_event("add_object_to_tab");
    lights_data.push_back(lght);
    object_list[lght->name] = lght;

    lightPos = vec3(0, 0, init_loc.z - 3);
    lght = new LightData();
    lght->name = SCENE_TAB_LIGHT_TITLE_1.toStdString();
    lght->light_name = "lightPosition[1]";
    lght->loc = vec4(lightPos.x, lightPos.y, lightPos.z, 1);
    lght->rot = vec4(0, 0, 0, 0);
    lght->scl = vec4(1, 1, 1, 1);
    lght->light_intensity = lightIntensity;
    lght->type = "light";
    lght->idx = 1;
    EventAdapter::shared->push_data(lght);
    EventAdapter::shared->trigger_event("add_object_to_tab");

    lights_data.push_back(lght);
    object_list[lght->name] = lght;

    lightPos = vec3(init_loc.x - 2, 0, init_loc.z);

    lght = new LightData();
    lght->name = SCENE_TAB_LIGHT_TITLE_2.toStdString();
    lght->light_name = "lightPosition[2]";
    lght->loc = vec4(lightPos.x, lightPos.y, lightPos.z, 1);
    lght->rot = vec4(0, 0, 0, 0);
    lght->scl = vec4(1, 1, 1, 1);
    lght->light_intensity = lightIntensity;
    lght->type = "light";
    lght->idx = 2;
    EventAdapter::shared->push_data(lght);
    EventAdapter::shared->trigger_event("add_object_to_tab");

    lights_data.push_back(lght);
    object_list[lght->name] = lght;

    lightPos = vec3(init_loc.x + 2, 0, init_loc.z);

    lght = new LightData();
    lght->name = SCENE_TAB_LIGHT_TITLE_3.toStdString();
    lght->light_name = "lightPosition[3]";
    lght->loc = vec4(lightPos.x, lightPos.y, lightPos.z, 1);
    lght->rot = vec4(0, 0, 0, 0);
    lght->scl = vec4(1, 1, 1, 1);
    lght->light_intensity = lightIntensity;
    lght->type = "light";
    lght->idx = 3;
    EventAdapter::shared->push_data(lght);
    EventAdapter::shared->trigger_event("add_object_to_tab");

    lights_data.push_back(lght);
    object_list[lght->name] = lght;
}
void Canvas::set_up_light()
{
    shader->setInt("lightCount", lights_data.size());
    for (int i = 0; i < lights_data.size(); i++) {
        shader->setVec3(string("lightPositions[") + std::to_string(i) + string("]"), vec3(lights_data[i]->loc.x, lights_data[i]->loc.y, lights_data[i]->loc.z));
        shader->setVec3(string("lightColors[") + std::to_string(i) + string("]"), lights_data[i]->light_intensity);
    }
}
void Canvas::initializeGL() {
    init_scene();
    if (this->background) {
        delete this->background;
    }
    this->background = new CanvasBackGround();
}
void Canvas::paintGL() {
    draw_scene();
}

void Canvas::resizeGL(int w, int h){
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
    f->glViewport(100, 200, w, h);
}

void Canvas::render_output()
{
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
    f->glActiveTexture(GL_TEXTURE0 + metallic_index);
    shader->setInt("metallicMap", metallic_index);
    if (!is_pipeline_on || PipelineManager::shared->output->metallic_map < 0) {
        shader->setFloat("i_metallic", i_metallic);
    }
    else {
        f->glBindTexture(GL_TEXTURE_2D, PipelineManager::shared->output->metallic_map);
    }
    f->glActiveTexture(GL_TEXTURE0 + roughness_index);
    shader->setInt("roughnessMap", roughness_index);
    if (!is_pipeline_on || PipelineManager::shared->output->roughness_map < 0) {
        shader->setFloat("i_roughness", i_roughness);
    }
    else {
        f->glBindTexture(GL_TEXTURE_2D, PipelineManager::shared->output->roughness_map);
    }
    f->glActiveTexture(GL_TEXTURE0);
}
void Canvas::init_scene() {
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
    shader = new Shader("resources/shaders/common_shader.vert", "resources/shaders/common_shader.frag");
    grid_shader = new Shader("resources/shaders/grid_shader.vert", "resources/shaders/grid_shader.frag");
}
void Canvas::draw_scene() {
    if (should_load) {
        model_itself->loadModel(model_path);
        should_load = false;
        EventAdapter::shared->trigger_event("new_mesh_loaded");
    }

    QOpenGLFunctions_3_3_Core* f = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(QOpenGLContext::currentContext());
    f->glClearColor(0.231f, 0.231f, 0.231f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //准备相机
    mat4 projection;
    if (is_ortho) {
        projection = glm::ortho(-direct_zoom_controller, direct_zoom_controller, -direct_zoom_controller, direct_zoom_controller, 0.1f, 45.0f);
    }
    else {
        projection = glm::perspective(radians(direct_zoom_controller), (float)width() / (float)height(), 0.1f, 45.0f);
    }
    mat4 view = camera->GetViewMatrix();
    mat4 model_m(1);
    ObjectData* object_dat = object_list["object"];
    if (object_dat != nullptr) {
        model_m = glm::translate(model_m, vec3(object_dat->loc.x, object_dat->loc.y, object_dat->loc.z));
        model_m = glm::scale(model_m, vec3(object_dat->scl.x, object_dat->scl.y, object_dat->scl.z));
    }
    else {
        model_m = glm::translate(model_m, init_loc);
        model_m = glm::scale(model_m, vec3(0.2f, 0.2f, 0.2f));
    }

    model_m = glm::rotate(model_m, glm::radians(-90.0f), vec3(0, 1, 0));
    
    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    grid_shader->use();
    grid_shader->setMat4("model", model_m);
    grid_shader->setMat4("view", view);
    grid_shader->setMat4("projection", projection);
    grid_shader->setFloat("near", 0.1f);
    grid_shader->setFloat("far", 100.0f);
    
    // 绘制背景
    background->Draw();

    f->glClear(GL_DEPTH_BUFFER_BIT);

    shader->use();
    set_up_light();
    shader->setFloat("ao", 1.0f);
    shader->setMat4("model", model_m);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setVec3("camPos", camera->Position);
    shader->setBool("is_pipeline_on", is_pipeline_on);

    render_output();

    if (model_itself) {
        model_itself->Draw(*shader);
    }
}

void Canvas::mousePressEvent(QMouseEvent* e)
{
    setFocusPolicy(Qt::StrongFocus);
    if (e->button() == Qt::MiddleButton) {
        view_state |= 0x1;
        prev_cursor = e->globalPos();
    }
}

void Canvas::keyPressEvent(QKeyEvent* e)
{
    int key = e->key();

    if (key == Qt::Key_Shift) {
        view_state |= 0x2;
    }
    else if (key == Qt::Key_O) {
        if (!setted) {

        }
    }
}

void Canvas::keyReleaseEvent(QKeyEvent* e)
{
    int key = e->key();
    if (key == Qt::Key_Shift) {
        view_state &= 0xFD;
    }
}

void Canvas::mouseMoveEvent(QMouseEvent* e)
{
    if (view_state != CANVAS_VIEW_NONE) {
        QPoint delta = e->globalPos() - prev_cursor;
        prev_cursor = e->globalPos();
        if (view_state == CANVAS_VIEW_MOVE) {
            //qDebug() << delta.x() << " " << delta.y();
            camera->Move(LEFT, ((float)delta.x()) * move_sensitivity);
            camera->Move(UP, ((float)delta.y()) * move_sensitivity);
            update();
        }else if (view_state == CANVAS_VIEW_ROTATE) {
            float x = delta.x() * rotate_sensitivity;
            float y = delta.y() * rotate_sensitivity;
            if (x > 0) {
                camera->camera_rotate(true, true, x);
            }
            else {
                camera->camera_rotate(false, true, -x);
            }
            if (y > 0) {
                camera->camera_rotate(true, false, y);
            }
            else {
                camera->camera_rotate(false, false, -y);
            }
            update();
        }
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::MiddleButton) {

        view_state &= 0xFE;
    }
}

void Canvas::wheelEvent(QWheelEvent* e)
{
    int y = e->angleDelta().y();
    float t = linear_zoom_controller - y;
    linear_zoom_controller = t < 7000 ? t:7000;
    direct_zoom_controller = zoom_project();
    update();
}

float Canvas::zoom_project()
{
    return view_min + pow(2, linear_zoom_controller / 1000.0f);
}

void Canvas::frame_consumer(float delta)
{
    update();
}


