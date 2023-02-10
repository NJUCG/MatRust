#include "PipelineManager.h"
PipelineManager* PipelineManager::shared;

void PipelineManager::init_pipeline()
{
    PipelineManager::shared = new PipelineManager();
}

void PipelineManager::register_responder(PipelineManagerResponder* responder)
{
    this->responder = responder;
}

PipelineManager::PipelineManager()
{
    this->output = new PipelineOutput();
}

PipelineManager::PipelineManager(PipelineManagerResponder* responder)
{
	this->responder = responder;
	this->layers = vector<Layer*>();
}

void PipelineManager::frame_consumer(float msc)
{
	if (state != Occupied) {
		current_time += msc;
	}
}

void PipelineManager::update_current(float c)
{
	current_time = c;
}

void PipelineManager::register_layer(Layer* layer)
{
	layers.push_back(layer);
}

void PipelineManager::start_pipeline(PipelineConfig* config)
{
    if (config == nullptr) {
        // 自行拼装Pipeline config
        config = new PipelineConfig();
        config->oc = config->sc = config->cc = 0.8f;
        config->textureWidth = config->textureHeight = 200;
        config->growTime = 20.0f;
        config->rh = 0.2f;
        config->backTexture = vector<vector<vec4>>(config->textureHeight, vector<vec4>(config->textureWidth, vec4()));
        config->metallic = vector<vector<float>>(config->textureHeight, vector<float>(config->textureWidth, 0.1f));
        config->roughness = vector<vector<float>>(config->textureHeight, vector<float>(config->textureWidth, 1.0f));
        config->curvature = CurvatureHelper::generate("resources/models/stranger/curve.png");
        for (int i = 0; i < 200; i++) {
            for (int j = 0; j < 200; j++) {
                config->backTexture[i][j] = vec4(255, 80, 60, 255);
            }
        }
    }
	this->config = config;
	state = Occupied;

    prepare_layers();
    dispatch_rules();
}

void PipelineManager::prepare_layers()
{
    units = vector<vector<vector<RustUnit>>>();
    
    Layer* oxide_layer = new OxideLayer();
    Layer* sulfur_layer = new SulfurLayer();

    //layers.push_back(oxide_layer);
    layers.push_back(sulfur_layer);

    units.push_back(vector<vector<RustUnit>>(config->textureHeight, vector<RustUnit>(config->textureWidth, RustUnit())));
}

void PipelineManager::dispatch_rules()
{
    for (int i = 0; i < layers.size(); i++) {
        Layer* layer = layers[i];
        layer->accept_rules(config);
    }
}

void PipelineManager::generate_rust(double delta)
{
    if (state != Occupied) {
        return;
    }
	int ls = layers.size();
	for (int i = 0; i < ls; i++) {
		Layer* layer = layers[i];

		layer->rust(delta);
	}
    merge_layers();
}

void PipelineManager::merge_layers()
{
    vector<vector<vec4>> diffData = vector<vector<vec4>>(config->textureHeight, vector<vec4>(config->textureWidth, vec4()));
    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
    
    for (int i = 0; i < layers.size(); i++) {
        units[i] = layers[i]->get_units();
    }
    if (PipelineManager::shared->output->diffuse_map > 0) {
        GLuint* addr = (GLuint*)&(PipelineManager::output->diffuse_map);
        f->glDeleteTextures(1, addr);
    }
    if (PipelineManager::shared->output->metallic_map > 0) {
        GLuint* addr = (GLuint*)&(PipelineManager::output->metallic_map);
        f->glDeleteTextures(1, addr);
    }
    if (PipelineManager::shared->output->roughness_map > 0) {
        GLuint* addr = (GLuint*)&(PipelineManager::output->roughness_map);
        f->glDeleteTextures(1, addr);
    }

    // UI界面搭建
    // 参数面板，预览框，素材库，状态栏
    // 颗粒感法向图
    // 规则
    // 材质迁移

    int layerSize = layers.size();

    ///反射率这只提供一个F0材质，其余在shader中
    for (int i = 0; i < config->textureHeight; i++) {
        for (int j = 0; j < config->textureWidth; j++) {
            vec4 color(0, 0, 0, 0);
            float lightIntensity = 1;
            for (int k = 0; k < layerSize; k++) {
                RustUnit unit = units[k][i][j];
                float transmittance = (float)getTransmittance(unit.composition, unit.thickness);
                
                color = color + unit.color * (1 - transmittance) * lightIntensity;
                lightIntensity *= transmittance;
            }
            color = color + config->backTexture[i][j] * lightIntensity;
            color.w = 255;
            /*
            if (color.x != 255) {
                qDebug() << color.x << " " << color.y << " " << color.z << " " << color.w;
            }*/
            
            diffData[i][j] = color;
        }
    }
    output->diffuse_map = bind4Map(diffData);

    for (int i = 0; i < config->textureHeight; i++) {
        for (int j = 0; j < config->textureWidth; j++) {
            vec4 color(0, 0, 0, 0);
            float lightIntensity = 1;
            for (int k = layerSize - 1; k >= 0; k--) {
                RustUnit unit = units[k][i][j];
                float transmittance = (float)getTransmittance(unit.composition, unit.thickness);
                color = color + vec4(0, 0, unit.roughness * (1 - transmittance) * lightIntensity, 0);
                lightIntensity *= transmittance;
            }
            color = color + vec4(0, 0, config->roughness[i][j], 0) * lightIntensity;
            color.z = color.z * 255;
            diffData[i][j] = color;
        }
    }
    output->roughness_map = bind4Map(diffData);

    for (int i = 0; i < config->textureHeight; i++) {
        for (int j = 0; j < config->textureWidth; j++) {
            vec4 color(0, 0, 0, 0);
            float lightIntensity = 1;
            for (int k = layerSize - 1; k >= 0; k--) {
                RustUnit unit = units[k][i][j];
                float transmittance = (float)getTransmittance(unit.composition, unit.thickness);
                color = color + vec4(0, 0, unit.metallic * (1 - transmittance) * lightIntensity, 0);
                lightIntensity *= transmittance;
            }
            color = color + vec4(0, 0, config->metallic[i][j], 0) * lightIntensity;
            color.z = color.z * 255;
            diffData[i][j] = color;
        }
    }
    output->metallic_map = bind4Map(diffData);

    if (config->generateImage) {
        config->generateImage = false;
    }
}

int PipelineManager::bind4Map(vector<vector<vec4>> lists)
{
    if (lists.size() == 0) {
        return -1;
    }
    int rowSize = lists.size();
    int colSize = lists[0].size();

    unsigned char* data = new unsigned char[rowSize * colSize * 4];

    for (int i = 0; i < rowSize; i++) {
        for (int j = 0; j < colSize; j++) {
            int cur_idx = (i * colSize + j) * 4;
            data[cur_idx] = lists[i][j].x;
            data[cur_idx + 1] = lists[i][j].y;
            data[cur_idx + 2] = lists[i][j].z;
            data[cur_idx + 3] = lists[i][j].w;
        }
    }

    QOpenGLExtraFunctions* f = QOpenGLContext::currentContext()->extraFunctions();
    GLuint texture_id = -1;
    f->glGenTextures(1, &texture_id);

    f->glBindTexture(GL_TEXTURE_2D, texture_id);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, colSize, rowSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    f->glGenerateMipmap(GL_TEXTURE_2D);

    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture_id;
}

float PipelineManager::getTransmittance(FilmComposition composition, float thickness)
{
    // absorption coefficient
    double ac = 0;
    switch (composition) {
    case CuI_O:
        ac = -5.2;
    case CuII_O:
        break;
    case CuSO4:
        ac = -4.6;
    case CSA:
        break;
    }
    return exp(ac * thickness);
}
