package com.rust.view.window;

import org.lwjgl.nuklear.NkImage;

public class WindowImage extends NodeObject{
    public boolean isData = false;
    public Integer textureId;
    public String name;
    public String path;
    public NkImage image;
    public int width,height;
    public float[][] data = null;
}
