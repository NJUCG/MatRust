package com.rust.view.window;

import com.jogamp.opengl.util.GLBuffers;
import com.rust.model.FilmComposition;
import com.rust.util.Texture;
import org.lwjgl.PointerBuffer;
import org.lwjgl.nuklear.NkColorf;

import java.nio.ByteBuffer;
import java.util.*;

public class LayerInfo extends NodeObject{
    public String type;
    public String name;
    public Dictionary<String, WindowImage> imgDictionary = new Hashtable<>();
    public FilmComposition composition;
    public NkColorf colorf;
    public Float per;
    public ByteBuffer treeBuffer = GLBuffers.newDirectByteBuffer(1);
    public PointerBuffer progressBuffer = PointerBuffer.allocateDirect(1);
    public LayerInfo(String name){
        this.name = name;
        per = 0f;
    }
    public void copy(LayerInfo info){
        id = info.id;
        name = info.name;
    }
    public List<Integer> connectedSlotId = new ArrayList<>();
}
