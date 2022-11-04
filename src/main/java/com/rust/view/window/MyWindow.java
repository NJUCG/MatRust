package com.rust.view.window;

import com.jogamp.opengl.util.GLBuffers;
import org.lwjgl.PointerBuffer;

import java.nio.ByteBuffer;
import java.util.Dictionary;
import java.util.Hashtable;

public class MyWindow {
    public int seed = -1;
    public int nextSeed(){
        seed = seed + 1;
        return seed;
    }
    protected Dictionary<String,ByteBuffer> byteBufferDictionary = new Hashtable<>();
    public ByteBuffer newByteBuffer(String name){
        if(byteBufferDictionary.get(name) == null){
            ByteBuffer temp = GLBuffers.newDirectByteBuffer(1);
            byteBufferDictionary.put(name,temp);
            return temp;
        }
        return byteBufferDictionary.get(name);
    }
    public ByteBuffer newByteBuffer(String name,int size){
        if(byteBufferDictionary.get(name) == null){
            ByteBuffer temp = GLBuffers.newDirectByteBuffer(size);
            byteBufferDictionary.put(name,temp);
            return temp;
        }
        return byteBufferDictionary.get(name);
    }
    public ByteBuffer getBBuffer(String name){
        return byteBufferDictionary.get(name);
    }

    protected Dictionary<String, PointerBuffer> pointerBufferDictionary = new Hashtable<>();
    public PointerBuffer newPointerBuffer(String name,int cap){
        if(pointerBufferDictionary.get(name) == null){
            PointerBuffer temp = PointerBuffer.allocateDirect(cap);
            pointerBufferDictionary.put(name,temp);
            return temp;
        }
        return pointerBufferDictionary.get(name);
    }
    public PointerBuffer getPBuffer(String name){
        return pointerBufferDictionary.get(name);
    }

    // args
    public Dictionary<String,WindowArg> windowArgDictionary = new Hashtable<>();
    public WindowArg newWindowArg(String name){
        WindowArg temp = new WindowArg();
        windowArgDictionary.put(name,temp);
        return temp;
    }
    public WindowArg getWindowArg(String name){
        return windowArgDictionary.get(name);
    }
}
