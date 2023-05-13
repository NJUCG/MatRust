#pragma once
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>
#include "Layer.h"
#include"PipelineManagerResponder.h"
#include "TimeMachineResponder.h"
#include"PipelineOutput.h"
#include"UIModel.h"
#include"OxideLayer.h"
#include"SulfurLayer.h"
#include"CurvatureHelper.h"
#include"CustomPerlinLayer.h"
#include"CustomDPDLayer.h"
#include"EventAdapter.h"
#include"ImageHelper.h"
#include"NormalDisturbHelper.h"

enum PipelineState {
	Occupied,
	Open
};

class PipelineManager: public TimeMachineResponder, public EventResponder
{
public:
	static PipelineManager* shared;
	static void init_pipeline();
	void register_responder(PipelineManagerResponder*);
	PipelineManager();
	PipelineManager(PipelineManagerResponder*);
	void on_trigger(string) override;
	void frame_consumer(float) override;
	void update_current(float) override;
	void register_layer(Layer*);
	void start_pipeline(PipelineConfig* config = nullptr);
	void clear_pipeline();
	void pause_pipeline();
	void continue_pipeline();
	void prepare_layers();
	void dispatch_rules();
	PipelineState state = Open;
	PipelineOutput* output = nullptr;
	void generate_rust(double);
	static int bind4Map(vector<vector<vec4>>);
	void save_output(QString);
	void save_pic(QString, vector<vector<vec4>>*);
protected:
	vector<Layer*> layers;
	vector<vector<vector<RustUnit>>> units;
	PipelineManagerResponder* responder;
	PipelineConfig* config = nullptr;

	float current_time = 0.0f;
	void merge_layers();
	float getTransmittance(FilmComposition composite, float thick);
	float getTransmittance(float ac, float thick);

	vector<vector<vec4>>* diffuse_data = nullptr;
	vector<vector<vec4>>* roughness_data = nullptr;
	vector<vector<vec4>>* metallic_data = nullptr;
	vector<vector<vec4>>* normal_disturb_data = nullptr;
	vector<vector<vec4>>* height_data = nullptr;
	vector<vector<vec4>>* noise_data = nullptr;

};
