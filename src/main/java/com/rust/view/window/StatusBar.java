package com.rust.view.window;

import com.jogamp.opengl.util.GLBuffers;
import com.rust.util.UIUtil;
import com.rust.view.IAction;
import com.rust.view.window.style.Style;
import org.lwjgl.PointerBuffer;
import org.lwjgl.nuklear.*;
import org.lwjgl.system.MemoryStack;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.List;

import static org.lwjgl.nuklear.Nuklear.*;
import static org.lwjgl.nuklear.Nuklear.nk_end;

public class StatusBar {
    public static StatusBar instance = new StatusBar();
    public NkColor separatorColor;
    public IAction onStart;
    public IAction onClear;
    private StatusBar(){
        separatorColor = NkColor.create().r((byte) 0x0).g((byte) 0x0).b((byte) 0x0).a((byte)59);
        rectW = 200;
        rectH = 300;
        sliderW = 200;
        sliderH = 25;
        slW = 70;
        sliderSize = new int[2];
        sliderSize[0] = slW;
        sliderSize[1] = sliderW;
        layerInfoList.add(new LayerInfo("Oxide"));
        layerInfoList.add(new LayerInfo("Sulfur"));
    }
    public void initAfterOpenGL(){
        startBtn = UIUtil.create_image("start.png");
        clearBtn = UIUtil.create_image("clear.png");
    }

    public int rectW,rectH;
    public int sliderW,sliderH,slW;
    public int[] sliderSize;
    public ByteBuffer layerBuffer = GLBuffers.newDirectByteBuffer(1);
    public ByteBuffer timeBuffer = GLBuffers.newDirectByteBuffer(1);
    public ByteBuffer playerBuffer = GLBuffers.newDirectByteBuffer(1);
    public PointerBuffer timeProgBuffer = PointerBuffer.allocateDirect(1);
    public List<LayerInfo> layerInfoList = new ArrayList<>();
    public static int seed = -1;
    public NkImage startBtn;
    public NkImage clearBtn;
    public static int nextSeed(){
        seed = seed+1;
        return seed;
    }
    public void layout(NkContext ctx, int x, int y){
        seed = -1;
        try(MemoryStack stack = MemoryStack.stackPush()){
            ctx = Style.style.statusStyle(ctx);
            NkRect rect = NkRect.malloc(stack);
            if (nk_begin(
                    ctx,
                    "status",
                    nk_rect(x, y, rectW, rectH, rect),
                    NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE
            )) {
                if(nk_tree_push_hashed(ctx,NK_TREE_NODE,"Time",NK_MINIMIZED,timeBuffer,nextSeed())){
                    UIUtil.ui_widget(ctx,25,new float[]{0.1f,0.6f,0.3f});
                    nk_progress(ctx, timeProgBuffer,100,false);
                    nk_label(ctx, Float.toString((float) timeProgBuffer.get(0)),NK_TEXT_LEFT);
                    nk_tree_pop(ctx);
                }
                if(nk_tree_push_hashed(ctx,NK_TREE_NODE,"Layers",NK_MINIMIZED,layerBuffer,nextSeed())){
                    for (LayerInfo inf:layerInfoList
                         ) {
                        addLayer(ctx,inf);
                    }
                    nk_tree_pop(ctx);
                }
                if(nk_tree_push_hashed(ctx,NK_TREE_NODE,"Player",NK_MINIMIZED,playerBuffer,nextSeed())){
                    UIUtil.ui_static_widget(ctx,40,new float[]{8,40,8,40,16});
                    if(nk_button_image(ctx,startBtn)){
                        onStart.act();
                    }
                    nk_spacing(ctx,1);
                    if(nk_button_image(ctx,clearBtn)){
                        onClear.act();
                    }
                    nk_spacing(ctx,1);
                    nk_tree_pop(ctx);
                }
            }
            nk_end(ctx);
        }
    }
    public void addLayer(NkContext ctx,LayerInfo info){
        if(nk_tree_push_hashed(ctx,NK_TREE_NODE, info.name,NK_MINIMIZED, info.treeBuffer,nextSeed())){
            nk_layout_row(ctx,NK_DYNAMIC,20,new float[]{0.1f,0.6f,0.3f});
            nk_spacing(ctx,1);
            nk_progress(ctx, info.progressBuffer,100,false);
            nk_label(ctx, info.per.toString(),NK_TEXT_LEFT);
            info.per = (float) info.progressBuffer.get(0);
            nk_tree_pop(ctx);
        }
    }

}
