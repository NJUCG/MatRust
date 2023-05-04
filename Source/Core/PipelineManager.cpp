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
    if (state != Open) {
        return;
    }
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
    output->normal_noise_map = ImageHelper::TextureFromFile("test_pat.png", "D:/rust/CRust/QtWidgetsApplication1/QtWidgetsApplication1");
}

void PipelineManager::clear_pipeline()
{
    for (Layer* layer : layers) {
        delete layer;
    }
    layers.clear();

    units.clear();

    state = Open;
}

void PipelineManager::pause_pipeline()
{
}

void PipelineManager::continue_pipeline()
{
}

void PipelineManager::prepare_layers()
{
    units = vector<vector<vector<RustUnit>>>();
    for (int i = 0; i < config->layers.size(); i++) {
        LayerConfig* c = config->layers[i];
        if (c->alg_type == "perlin") {
            CustomPerlinLayer* layer = new CustomPerlinLayer(c);
            layers.push_back(layer);
            units.push_back(vector<vector<RustUnit>>(config->textureHeight, vector<RustUnit>(config->textureWidth, RustUnit())));
        }
        else if (c->alg_type == "DPD") {
            CustomDPDLayer* layer = new CustomDPDLayer(c);
            layers.push_back(layer);
            units.push_back(vector<vector<RustUnit>>(config->textureHeight, vector<RustUnit>(config->textureWidth, RustUnit())));
        }
    }
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

    int layerSize = layers.size();

    for (int i = 0; i < config->textureHeight; i++) {
        for (int j = 0; j < config->textureWidth; j++) {
            vec4 color(0, 0, 0, 0);
            float lightIntensity = 1;
            for (int k = 0; k < layerSize; k++) {
                RustUnit unit = units[k][i][j];
                float transmittance = 0;
                if (unit.composition == FilmComposition::Custom) {
                    transmittance = (float)getTransmittance(unit.ac, unit.thickness * unit.diffuse_spd);
                }
                else {
                    transmittance = (float)getTransmittance(unit.composition, unit.thickness);
                }
                color = color + unit.color * (1 - transmittance) * lightIntensity;
                lightIntensity *= transmittance;
            }
            color = color + config->backTexture[i][j] * lightIntensity;
            color.w = 255;
            diffData[i][j] = color;
        }
    }
    output->diffuse_map = bind4Map(diffData);

    for (int i = 0; i < config->textureHeight; i++) {
        for (int j = 0; j < config->textureWidth; j++) {
            vec4 color(0, 0, 0, 0);
            float lightIntensity = 1;
            bool flag = false;
            for (int k = layerSize - 1; k >= 0; k--) {
                RustUnit unit = units[k][i][j];
                float transmittance = 0;
                if (unit.composition == FilmComposition::Custom) {
                    transmittance = (float)getTransmittance(unit.ac, unit.thickness * 10);
                }
                else {
                    transmittance = (float)getTransmittance(unit.composition, unit.thickness * 10);
                }
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
                float transmittance = 0;
                if (unit.composition == FilmComposition::Custom) {
                    transmittance = (float)getTransmittance(unit.ac, unit.thickness);
                }
                else {
                    transmittance = (float)getTransmittance(unit.composition, unit.thickness);
                }
                color = color + vec4(0, 0, unit.metallic * (1 - transmittance) * lightIntensity, 0);
                lightIntensity *= transmittance;
            }
            color = color + vec4(0, 0, config->metallic[i][j], 0) * lightIntensity;
            color.z = color.z * 255;
            diffData[i][j] = color;
        }
    }
    output->metallic_map = bind4Map(diffData);

    for (int i = 0; i < config->textureHeight; i++) {
        for (int j = 0; j < config->textureWidth; j++) {
            diffData[i][j] = vec4(0, 0, 0, 0);
            for (int k = layerSize - 1; k >= 0; k--) {
                RustUnit unit = units[k][i][j];
                if (unit.has_disturb) {
                    diffData[i][j] = vec4(255, 255, 255, 255);
                    break;
                }
            }
        }
    }
    output->normal_disturb_map = bind4Map(diffData);

    float max_depth = 50;
    vector<vector<float>> depth_vec(config->textureHeight, vector<float>(config->textureWidth, 0));
    for (int i = 0; i < config->textureHeight; i++) {
        for (int j = 0; j < config->textureWidth; j++) {
            for (int k = layerSize - 1; k >= 0; k--) {
                RustUnit unit = units[k][i][j];
                depth_vec[i][j] += unit.thickness;
            }
            //max_depth = std::max(max_depth, depth_vec[i][j]);
        }
    }
    for (int i = 0; i < config->textureHeight; i++) {
        for (int j = 0; j < config->textureWidth; j++) {
            float d = 255 * depth_vec[i][j] / max_depth;
            diffData[i][j] = vec4(d, d, d, 0);
        }
    }
    output->depth_map = bind4Map(diffData);

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

void PipelineManager::save_output(QString dir)
{
    if (!output) {
        return;
    }
    long long int h = config->textureHeight;
    long long int w = config->textureWidth;
    unsigned char* diffData = new unsigned char[h * w * 3];
    int layerSize = layers.size();
    int idx = 0;
    for (int i = 0; i < config->textureHeight; i++) {
        for (int j = 0; j < config->textureWidth; j++) {
            vec4 color(0, 0, 0, 0);
            float lightIntensity = 1;
            for (int k = 0; k < layerSize; k++) {
                RustUnit unit = units[k][i][j];
                float transmittance = 0;
                if (unit.composition == FilmComposition::Custom) {
                    transmittance = (float)getTransmittance(unit.ac, unit.thickness * unit.diffuse_spd);
                }
                else {
                    transmittance = (float)getTransmittance(unit.composition, unit.thickness);
                }
                color = color + unit.color * (1 - transmittance) * lightIntensity;
                lightIntensity *= transmittance;
            }
            color = color + config->backTexture[i][j] * lightIntensity;
            color.w = 255;
            diffData[idx++] = color.r;
            diffData[idx++] = color.g;
            diffData[idx++] = color.b;
        }
    }
    ImageHelper::save_pic((dir + "/" + "diffuse.png").toStdString(), diffData, 3, w, h);
    idx = 0;
    for (int i = 0; i < config->textureHeight; i++) {
        for (int j = 0; j < config->textureWidth; j++) {
            vec4 color(0, 0, 0, 0);
            float lightIntensity = 1;
            for (int k = layerSize - 1; k >= 0; k--) {
                RustUnit unit = units[k][i][j];
                float transmittance = 0;
                if (unit.composition == FilmComposition::Custom) {
                    transmittance = (float)getTransmittance(unit.ac, unit.thickness);
                }
                else {
                    transmittance = (float)getTransmittance(unit.composition, unit.thickness);
                }
                color = color + vec4(0, 0, unit.roughness * (1 - transmittance) * lightIntensity, 0);
                lightIntensity *= transmittance;
            }
            color = color + vec4(0, 0, config->roughness[i][j], 0) * lightIntensity;
            color.z = color.z * 255;
            diffData[idx++] = color.r;
            diffData[idx++] = color.g;
            diffData[idx++] = color.b;
        }
    }
    ImageHelper::save_pic((dir + "/" + "roughness.png").toStdString(), diffData, 3, w, h);
    idx = 0;
    for (int i = 0; i < config->textureHeight; i++) {
        for (int j = 0; j < config->textureWidth; j++) {
            vec4 color(0, 0, 0, 0);
            float lightIntensity = 1;
            for (int k = layerSize - 1; k >= 0; k--) {
                RustUnit unit = units[k][i][j];
                float transmittance = 0;
                if (unit.composition == FilmComposition::Custom) {
                    transmittance = (float)getTransmittance(unit.ac, unit.thickness);
                }
                else {
                    transmittance = (float)getTransmittance(unit.composition, unit.thickness);
                }
                color = color + vec4(0, 0, unit.metallic * (1 - transmittance) * lightIntensity, 0);
                lightIntensity *= transmittance;
            }
            color = color + vec4(0, 0, config->metallic[i][j], 0) * lightIntensity;
            color.z = color.z * 255;
            diffData[idx++] = color.r;
            diffData[idx++] = color.g;
            diffData[idx++] = color.b;
        }
    }
    ImageHelper::save_pic((dir + "/" + "metallic.png").toStdString(), diffData, 3, w, h);
    idx = 0;
    for (int i = 0; i < config->textureHeight; i++) {
        for (int j = 0; j < config->textureWidth; j++) {
            bool flag = false;
            for (int k = layerSize - 1; k >= 0; k--) {
                RustUnit unit = units[k][i][j];
                if (unit.has_disturb) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                diffData[idx++] = 255;
                diffData[idx++] = 255;
                diffData[idx++] = 255;
            }
            else {
                diffData[idx++] = 0;
                diffData[idx++] = 0;
                diffData[idx++] = 0;
            }
        }
    }
    ImageHelper::save_pic((dir + "/" + "normal_disturb.png").toStdString(), diffData, 3, w, h);
    idx = 0;
    float max_depth = 50;
    vector<vector<float>> depth_vec(config->textureHeight, vector<float>(config->textureWidth, 0));
    for (int i = 0; i < config->textureHeight; i++) {
        for (int j = 0; j < config->textureWidth; j++) {
            for (int k = layerSize - 1; k >= 0; k--) {
                RustUnit unit = units[k][i][j];
                depth_vec[i][j] += unit.thickness;
            }
        }
    }
    for (int i = 0; i < config->textureHeight; i++) {
        for (int j = 0; j < config->textureWidth; j++) {
            float d = 255 * depth_vec[i][j] / max_depth;
            diffData[idx++] = d;
            diffData[idx++] = d;
            diffData[idx++] = d;
        }
    }
    ImageHelper::save_pic((dir + "/" + "depth.png").toStdString(), diffData, 3, w, h);
    
    delete[] diffData;
}

float PipelineManager::getTransmittance(FilmComposition composition, float thickness)
{
    // absorption coefficient
    double ac = 0;
    switch (composition) {
    case CuI_O:
        ac = -0.52;
    case CuII_O:
        break;
    case CuSO4:
        ac = -4.6;
    case CSA:
        break;
    }
    return exp(ac * thickness);
}

float PipelineManager::getTransmittance(float ac, float thick)
{
    return exp(ac * thick);
}
