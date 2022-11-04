package com.rust.util;

import com.rust.model.FilmComposition;
import glm.vec._4.Vec4;

import java.util.Dictionary;
import java.util.Hashtable;

public class ColorHelper {
    public static Dictionary<FilmComposition, Vec4> colors = new Hashtable<>();
    private static ColorHelper shared = new ColorHelper();
    private ColorHelper(){
        colors.put(FilmComposition.CuII_O,new Vec4(144,38,37,255));
        colors.put(FilmComposition.CuSO4,new Vec4(10,77,60,255));
        colors.put(FilmComposition.CSA,new Vec4(4,121,214,255));
    }

    public static Vec4 colorOf(FilmComposition composition){
        return shared.colors.get(composition);
    }
}
