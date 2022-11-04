package com.rust.model;


import com.jogamp.opengl.util.GLBuffers;
import com.rust.model.layers.*;
import com.rust.model.layers.LayerType;
import com.rust.util.GlmExtension;
import com.rust.util.ImageHelper;
import com.rust.util.TimeManager;
import com.rust.view.window.LayerInfo;
import com.rust.view.window.StatusBar;
import com.rust.view.window.TextileMachine;
import com.sun.jna.platform.win32.OaIdl;
import glm.vec._3.Vec3;
import glm.vec._4.Vec4;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.sql.Time;
import java.util.ArrayList;
import java.util.Dictionary;
import java.util.Hashtable;
import java.util.List;

import static org.lwjgl.opengl.GL33C.*;
/**
 * 用于管理整个管线。
 */
public class PipelineManager {
    public static PipelineManager shared = new PipelineManager();
    public PipelineConfig config;
    public PipelineState state = PipelineState.OPEN;
    public PipelineOutput output = new PipelineOutput();
    /// 从高到低
    public List<BaseLayer> layers = new ArrayList<>();
    private PipelineManager(){

    }
    public void clear(){
        output = new PipelineOutput();
        layers.clear();
    }
    public void startPipeline(PipelineConfig config){
        System.out.println("Pipeline started");
        state = PipelineState.OCCUPIED;
        this.config = config;
        currentTime = 0;
        try {
            prepareLayers();
            dispatchRules();
        }catch (Exception e){
            System.out.println("Pipeline failed.");
            e.printStackTrace();
        }
    }

    public int bindMap(List<List<Vec3>> lists) throws Exception {
        if(lists.isEmpty()){
            throw new Exception("Empty list");
        }
        int rowSize = lists.size();
        int colSize = lists.get(0).size();
        int size = rowSize * colSize * 3;

        byte[] bytes = new byte[size];

        for(int i=0;i<lists.size();i++){
            for(int j=0;j<lists.get(i).size();j++){
                Vec3 v = lists.get(i).get(j);
                int base = (i * colSize + j) * 3;
                bytes[base] = (byte)  v.x;
                bytes[base+1] = (byte)  v.y;
                bytes[base+2] = (byte)  v.z;
            }
        }

        ByteBuffer data = GLBuffers.newDirectByteBuffer(bytes);
        return bindMap(data,800,600,GL_RGB);
    }

    public int bind4Map(List<List<Vec4>> lists) throws Exception {
        if(lists.isEmpty()){
            throw new Exception("Empty list");
        }
        int rowSize = lists.size();
        int colSize = lists.get(0).size();
        int size = rowSize * colSize * 4;

        byte[] bytes = new byte[size];

        for(int i=0;i<lists.size();i++){
            for(int j=0;j<lists.get(i).size();j++){
                Vec4 v = lists.get(i).get(j);
                int base = (i * colSize + j) * 4;
                bytes[base] = (byte)  v.x;
                bytes[base+1] = (byte)  v.y;
                bytes[base+2] = (byte)  v.z;
                bytes[base+3] = (byte)  v.w;
            }
        }

        ByteBuffer data = GLBuffers.newDirectByteBuffer(bytes);
        return bindMap(data,config.textureWidth,config.textureHeight,GL_RGBA);
    }
    public int bind4Map(Vec4[][] lists) throws Exception {
        if(lists.length == 0){
            throw new Exception("Empty list");
        }
        int rowSize = lists.length;
        int colSize = lists[0].length;
        int size = rowSize * colSize * 4;

        byte[] bytes = new byte[size];

        for(int i=0;i<rowSize;i++){
            for(int j=0;j<colSize;j++){
                Vec4 v = lists[i][j];
                int base = (i * colSize + j) * 4;
                bytes[base] = (byte)  v.x;
                bytes[base+1] = (byte)  v.y;
                bytes[base+2] = (byte)  v.z;
                bytes[base+3] = (byte)  v.w;
            }
        }

        ByteBuffer data = GLBuffers.newDirectByteBuffer(bytes);
        return bindMap(data,config.textureWidth,config.textureHeight,GL_RGBA);
    }
    public int bind4Map(Vec4[][] lists,int w,int h) throws Exception {
        if(lists.length == 0){
            throw new Exception("Empty list");
        }
        int rowSize = lists.length;
        int colSize = lists[0].length;
        int size = rowSize * colSize * 4;

        byte[] bytes = new byte[size];

        for(int i=0;i<rowSize;i++){
            for(int j=0;j<colSize;j++){
                Vec4 v = lists[i][j];
                int base = (i * colSize + j) * 4;
                bytes[base] = (byte)  v.x;
                bytes[base+1] = (byte)  v.y;
                bytes[base+2] = (byte)  v.z;
                bytes[base+3] = (byte)  v.w;
            }
        }

        ByteBuffer data = GLBuffers.newDirectByteBuffer(bytes);
        return bindMap(data,w,h,GL_RGBA);
    }
    private int bindMap(ByteBuffer data,int w,int h,int format){
        IntBuffer texture_id = GLBuffers.newDirectIntBuffer(1);
        glGenTextures(texture_id);
        int[] widths = {w};
        int[] heights = {h};
        IntBuffer width = GLBuffers.newDirectIntBuffer(widths);
        IntBuffer height = GLBuffers.newDirectIntBuffer(heights);

        if(data!=null){
            glBindTexture(GL_TEXTURE_2D,texture_id.get(0));
            glTexImage2D(GL_TEXTURE_2D,0,format,width.get(0),height.get(0),0,format,GL_UNSIGNED_BYTE,data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            data.clear();
        }

        return texture_id.get(0);
    }

    public Dictionary<LayerType,BaseLayer> dict_layers = new Hashtable<>();
    private void prepareLayers(){
        List<LayerInfo> infoList = TextileMachine.instance.infoList;
        for (LayerInfo info : infoList
             ) {
            addLayer(info);
        }
        System.out.println(layers.size());
        units = new RustUnit[layers.size()][config.textureHeight][config.textureWidth];
    }
    private void addLayer(LayerInfo info){
        System.out.println(info.type);
        if(info.type.equals("OxideLayer")){
            BaseLayer t = new OxideLayer();
            layers.add(t);
            dict_layers.put(LayerType.Oxide,t);
        }else if(info.type.equals("SulfurLayer")){
            SulfideLayer t = new SulfideLayer();
            layers.add(t);
            dict_layers.put(LayerType.Sulfide,t);
        }
    }

    private void dispatchRules(){
        for (BaseLayer layer:
             layers) {
            layer.acceptRules(config);
        }
    }
    public volatile boolean shouldMerge = false;
    public float currentTime = 0;
    public void frameFun(){
        if (state == PipelineState.OCCUPIED){
            currentTime += TimeManager.shared.deltaTime;
            StatusBar.instance.timeProgBuffer.put(0, (long) (currentTime * 100/config.growTime));
            generateRust(TimeManager.shared.deltaTime);
            //if (shouldMerge){
            //    shouldMerge = false;
                try {
                    mergeLayers();
                }catch (Exception e){
                    e.printStackTrace();
                }
            //}
            if (currentTime > config.growTime){
                completePipeline();
            }
        }
    }
    public void generateRust(double deltaTime){
        for (BaseLayer layer:
                layers) {
            layer.rust(deltaTime);
        }
    }
    private RustUnit[][][] units;
    public void mergeLayers() throws Exception {
        Vec4[][] diffData = new Vec4[config.textureHeight][config.textureWidth];
        for(int i=0;i<layers.size();i++){
            units[i] = layers.get(i).getUnits();
        }
        if(PipelineManager.shared.output.diffuse_map > 0 ){
            glDeleteTextures(PipelineManager.shared.output.diffuse_map);
        }
        if(PipelineManager.shared.output.metallic_map > 0 ){
            glDeleteTextures(PipelineManager.shared.output.metallic_map);
        }
        if(PipelineManager.shared.output.roughness_map > 0 ){
            glDeleteTextures(PipelineManager.shared.output.roughness_map);
        }

        // UI界面搭建
        // 参数面板，预览框，素材库，状态栏
        // 颗粒感法向图
        // 规则
        // 材质迁移

        int layerSize = layers.size();

        ///反射率这只提供一个F0材质，其余在shader中
        for (int i = 0; i < config.textureHeight; i++) {
            for (int j = 0; j < config.textureWidth; j++) {
                Vec4 color = new Vec4(0,0,0,0);
                float lightIntensity = 1;
                for (int k=0; k< layerSize;k++){
                    RustUnit unit = units[k][i][j];
                    float transmittance = (float) getTransmittance(unit.composition,unit.thickness);
                    color = GlmExtension.add(color,GlmExtension.mul(unit.color, (1 - transmittance)*lightIntensity));
                    lightIntensity *= transmittance;
                }
                color = GlmExtension.add(color,GlmExtension.mul(config.backTexture[i][j],lightIntensity));
                diffData[i][j] = color;
                /*Vec4 color = config.backTexture[i][j];
                for (int k=layerSize-1; k>= 0;k--){
                    RustUnit unit = units[k][i][j];
                    if(unit.thickness>0){
                        color = unit.color;
                        break;
                    }
                }
                diffData[i][j] = color;*/
            }
        }
        output.diffuse_map = bind4Map(diffData);

        for (int i = 0; i < config.textureHeight; i++) {
            for (int j = 0; j < config.textureWidth; j++) {
                Vec4 color = new Vec4(0,0,0,0);
                float lightIntensity = 1;
                for(int k=layerSize-1;k>=0;k--){
                    RustUnit unit = units[k][i][j];
                    float transmittance = (float) getTransmittance(unit.composition,unit.thickness);
                    color = GlmExtension.add(color,new Vec4(0,0,unit.roughness*(1 - transmittance)*lightIntensity,0));
                    lightIntensity *= transmittance;
                }
                color = GlmExtension.add(color,GlmExtension.mul(new Vec4(0,0,config.roughness[i][j],0),lightIntensity));
                color.z = color.z * 255;
                diffData[i][j] = color;
            }
        }
        output.roughness_map = bind4Map(diffData);

        for (int i = 0; i < config.textureHeight; i++) {
            for (int j = 0; j < config.textureWidth; j++) {
                Vec4 color = new Vec4(0,0,0,0);
                float lightIntensity = 1;
                for(int k=layerSize-1;k>=0;k--){
                    RustUnit unit = units[k][i][j];
                    float transmittance = (float) getTransmittance(unit.composition,unit.thickness);
                    color = GlmExtension.add(color,new Vec4(0,0,unit.metallic*(1 - transmittance)*lightIntensity,0));
                    lightIntensity *= transmittance;
                }
                color = GlmExtension.add(color,GlmExtension.mul(new Vec4(0,0,config.metallic[i][j],0),lightIntensity));
                color.z = color.z * 255;
                diffData[i][j] = color;
            }
        }
        output.metallic_map = bind4Map(diffData);

        if (config.generateImage){
            config.generateImage =false;
            ImageHelper.saveAsPng(diffData,config.textureWidth,config.textureHeight,"src/main/resources"+config.diffPath);
        }
    }

    public void completePipeline(){
        state = PipelineState.OPEN;
        System.out.println("Pipeline succeed.");
    }

    /**
     * @param filmComposition
     * @param d
     * beer - lambert law
     * https://refractiveindex.info
     * 波长500nm（尽量，有些没数据）
     * 暂时不使用完全仿真的参数，而是使用相对正确的数据,这是由于一些冷门物质的参数难以找到，以及程序为方便实现做出的一些妥协
     * F0使用
     * @return
     */
    private double getTransmittance(FilmComposition filmComposition,float d){
        // absorption coefficient
        double ac = 0;
        switch (filmComposition){
            case CuI_O:
                ac = -1.2;
            case CuII_O:
                break;
            case CuSO4:
                ac = -4.6;
            case CSA:
                break;
        }
        return Math.exp(ac * d);
    }
    private double getF0(FilmComposition filmComposition,float d){
        // absorption coefficient
        switch (filmComposition){
            case CuI_O:
                return 0.15;
            case CuII_O:
                break;
        }
        return 0;
    }
}
