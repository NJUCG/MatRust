package com.rust.view.window.style;

import org.lwjgl.nuklear.*;
import org.lwjgl.system.MemoryStack;

import java.nio.ByteBuffer;

import static org.lwjgl.nuklear.Nuklear.*;

public class Style {
    public static Style style = new Style();
    public static NkColor gray50 = createColor(0,0,0,128);
    public static NkColor transparent = createColor(0,0,0,0);
    public static NkColor gray75 = createColor(0,0,0,192);
    public static NkColor white = createColor( 255, 255, 255, 255);
    public static NkColor black = createColor( 0, 0, 0, 255);
    public static NkColor grey00 = createColor( 20, 20, 20, 255);
    public static NkColor grey01 = createColor( 45, 45, 45, 255);
    public static NkColor grey02 = createColor( 70, 70, 70, 255);
    public static NkColor grey03 = createColor( 120, 120, 120, 255);
    public static NkColor grey04 = createColor( 140, 140, 140, 255);
    public static NkColor grey05 = createColor( 150, 150, 150, 255);
    public static NkColor grey06 = createColor( 160, 160, 160, 255);
    public static NkColor grey07 = createColor( 170, 170, 170, 255);
    public static NkColor grey08 = createColor(180, 180, 180, 255);
    public static NkColor grey09 = createColor( 185, 185, 185, 255);
    public static NkColor grey10 = createColor( 190, 190, 190, 255);
    public static NkColor grey11 = createColor( 200, 200, 200, 255);
    public static NkColor grey12 = createColor( 240, 240, 240, 255);
    public static NkColor blue1 = createColor( 80, 80, 200, 255);
    public static NkColor blue2 = createColor( 128, 196, 255, 255);
    public static NkColor blue3 = createColor( 64, 196, 255, 255);
    public static NkColor red = createColor( 255, 0, 0, 255);
    public static NkColor tab_grey = createColor( 61, 61, 61, 255);

    private Style(){

    }

    public NkContext toolBarStyle(NkContext ctx){
        try (MemoryStack stack = MemoryStack.stackPush()){
            int size = NkColor.SIZEOF * NK_COLOR_COUNT;
            ByteBuffer buffer = stack.malloc(size);
            NkColor.Buffer colors = new NkColor.Buffer(buffer);
            colors.put(NK_COLOR_WINDOW, grey00);
            colors.put(NK_COLOR_HEADER, gray75);
            colors.put(NK_COLOR_TEXT,white);
            colors.put(NK_COLOR_BORDER, transparent);
            colors.put(NK_COLOR_BUTTON, grey09);
            colors.put(NK_COLOR_BUTTON_HOVER, grey07);
            colors.put(NK_COLOR_BUTTON_ACTIVE, grey06);
            colors.put(NK_COLOR_TOGGLE, grey05);
            colors.put(NK_COLOR_TOGGLE_HOVER, grey03);
            colors.put(NK_COLOR_TOGGLE_CURSOR, grey10);
            colors.put(NK_COLOR_SELECT, grey06);
            colors.put(NK_COLOR_SELECT_ACTIVE, white);
            colors.put(NK_COLOR_SLIDER, grey12);
            colors.put(NK_COLOR_SLIDER_CURSOR, blue2);
            colors.put(NK_COLOR_SLIDER_CURSOR_HOVER, blue3);
            colors.put(NK_COLOR_SLIDER_CURSOR_ACTIVE, blue2);
            colors.put(NK_COLOR_PROPERTY, grey10);
            colors.put(NK_COLOR_EDIT, grey05);
            colors.put(NK_COLOR_EDIT_CURSOR, black);
            colors.put(NK_COLOR_COMBO, grey10);
            colors.put(NK_COLOR_CHART, grey06);
            colors.put(NK_COLOR_CHART_COLOR, grey01);
            colors.put(NK_COLOR_CHART_COLOR_HIGHLIGHT, red);
            colors.put(NK_COLOR_SCROLLBAR, grey08);
            colors.put(NK_COLOR_SCROLLBAR_CURSOR, grey04);
            colors.put(NK_COLOR_SCROLLBAR_CURSOR_HOVER, grey05);
            colors.put(NK_COLOR_SCROLLBAR_CURSOR_ACTIVE, grey06);
            colors.put(NK_COLOR_TAB_HEADER, grey08);
            nk_style_from_table(ctx,colors);

            NkStyleButton style = ctx.style().window().header().close_button();
            style.normal().data().color(transparent);
            style.active().data().color(transparent);
            style.hover().data().color(transparent);
            style = ctx.style().window().header().minimize_button();
            style.normal().data().color(transparent);
            style.active().data().color(transparent);
            style.hover().data().color(transparent);

            style = ctx.style().button();
            style.normal().data().color(transparent);
            style.active().data().color(transparent);
            style.hover().data().color(transparent);

            NkStyleWindow styleWindow = ctx.style().window();
            styleWindow.rounding(4);
        }
        return ctx;
    }
    public NkContext statusStyle(NkContext ctx){
        try (MemoryStack stack = MemoryStack.stackPush()){
            int size = NkColor.SIZEOF * NK_COLOR_COUNT;
            ByteBuffer buffer = stack.malloc(size);
            NkColor.Buffer colors = new NkColor.Buffer(buffer);
            colors.put(NK_COLOR_TEXT, black);
            colors.put(NK_COLOR_WINDOW, gray50);
            colors.put(NK_COLOR_HEADER, gray75);
            colors.put(NK_COLOR_TEXT,white);
            colors.put(NK_COLOR_BORDER, transparent);
            colors.put(NK_COLOR_BUTTON, grey09);
            colors.put(NK_COLOR_BUTTON_HOVER, grey07);
            colors.put(NK_COLOR_BUTTON_ACTIVE, grey06);
            colors.put(NK_COLOR_TOGGLE, grey05);
            colors.put(NK_COLOR_TOGGLE_HOVER, grey03);
            colors.put(NK_COLOR_TOGGLE_CURSOR, grey10);
            colors.put(NK_COLOR_SELECT, grey06);
            colors.put(NK_COLOR_SELECT_ACTIVE, white);
            colors.put(NK_COLOR_SLIDER, grey12);
            colors.put(NK_COLOR_SLIDER_CURSOR, blue2);
            colors.put(NK_COLOR_SLIDER_CURSOR_HOVER, blue3);
            colors.put(NK_COLOR_SLIDER_CURSOR_ACTIVE, blue2);
            colors.put(NK_COLOR_PROPERTY, grey10);
            colors.put(NK_COLOR_EDIT, grey05);
            colors.put(NK_COLOR_EDIT_CURSOR, black);
            colors.put(NK_COLOR_COMBO, grey10);
            colors.put(NK_COLOR_CHART, grey06);
            colors.put(NK_COLOR_CHART_COLOR, grey01);
            colors.put(NK_COLOR_CHART_COLOR_HIGHLIGHT, red);
            colors.put(NK_COLOR_SCROLLBAR, grey08);
            colors.put(NK_COLOR_SCROLLBAR_CURSOR, grey04);
            colors.put(NK_COLOR_SCROLLBAR_CURSOR_HOVER, grey05);
            colors.put(NK_COLOR_SCROLLBAR_CURSOR_ACTIVE, grey06);
            colors.put(NK_COLOR_TAB_HEADER, grey08);
            nk_style_from_table(ctx,colors);
            ctx.style().text().color(Style.style.createColor(stack,255,255,255,255));
            ctx.style().window().padding().set(10,10);
            NkStyleButton style = ctx.style().window().header().close_button();
            style.normal().data().color(transparent);
            style.active().data().color(transparent);
            style.hover().data().color(transparent);
            style = ctx.style().window().header().minimize_button();
            style.normal().data().color(transparent);
            style.active().data().color(transparent);
            style.hover().data().color(transparent);
        }
        return ctx;
    }
    public NkContext controllTabStyle(NkContext ctx){
        try (MemoryStack stack = MemoryStack.stackPush()){


            int size = NkColor.SIZEOF * NK_COLOR_COUNT;
            ByteBuffer buffer = stack.malloc(size);
            NkColor.Buffer colors = new NkColor.Buffer(buffer);
            colors.put(NK_COLOR_WINDOW, grey01);
            colors.put(NK_COLOR_HEADER, grey00);
            colors.put(NK_COLOR_TEXT,white);
            colors.put(NK_COLOR_BORDER, transparent);
            colors.put(NK_COLOR_BUTTON, grey03);
            colors.put(NK_COLOR_BUTTON_HOVER, grey02);
            colors.put(NK_COLOR_BUTTON_ACTIVE, grey01);
            colors.put(NK_COLOR_TOGGLE, grey05);
            colors.put(NK_COLOR_TOGGLE_HOVER, grey03);
            colors.put(NK_COLOR_TOGGLE_CURSOR, grey10);
            colors.put(NK_COLOR_SELECT, grey06);
            colors.put(NK_COLOR_SELECT_ACTIVE, white);
            colors.put(NK_COLOR_SLIDER, grey12);
            colors.put(NK_COLOR_SLIDER_CURSOR, blue2);
            colors.put(NK_COLOR_SLIDER_CURSOR_HOVER, blue3);
            colors.put(NK_COLOR_SLIDER_CURSOR_ACTIVE, blue2);
            colors.put(NK_COLOR_PROPERTY, grey10);
            colors.put(NK_COLOR_EDIT, grey05);
            colors.put(NK_COLOR_EDIT_CURSOR, black);
            colors.put(NK_COLOR_COMBO, grey03);
            colors.put(NK_COLOR_CHART, grey06);
            colors.put(NK_COLOR_CHART_COLOR, grey01);
            colors.put(NK_COLOR_CHART_COLOR_HIGHLIGHT, red);
            colors.put(NK_COLOR_SCROLLBAR, grey08);
            colors.put(NK_COLOR_SCROLLBAR_CURSOR, grey04);
            colors.put(NK_COLOR_SCROLLBAR_CURSOR_HOVER, grey05);
            colors.put(NK_COLOR_SCROLLBAR_CURSOR_ACTIVE, grey06);
            colors.put(NK_COLOR_TAB_HEADER, tab_grey);

            nk_style_from_table(ctx,colors);
            ctx.style().text().color(Style.style.createColor(stack,255,255,255,255));
            ctx.style().window().padding().set(10,10);
            NkStyleButton style = ctx.style().window().header().close_button();
            style.normal().data().color(transparent);
            style.active().data().color(transparent);
            style.hover().data().color(transparent);
            style = ctx.style().window().header().minimize_button();
            style.normal().data().color(transparent);
            style.active().data().color(transparent);
            style.hover().data().color(transparent);
            NkStyleCombo styleCombo = ctx.style().combo();
            styleCombo.rounding(2f);
            ctx.style().button().rounding(2f);
            NkStyleProperty property = ctx.style().property();
            property.normal().data().color(grey03);
            property.hover().data().color(grey03);
            property.active().data().color(grey03);
            style = property.dec_button();
            style.normal().data().color(grey03);
            style.hover().data().color(grey03);
            style.active().data().color(grey03);
            style = property.inc_button();
            style.normal().data().color(grey03);
            style.hover().data().color(grey03);
            style.active().data().color(grey03);

            NkStyleEdit edit = property.edit();
            edit.normal().data().color(grey03);
            edit.hover().data().color(grey03);
            edit.active().data().color(grey03);

            ctx.style().window().border_color().set(white);
            ctx.style().window().border(2);
            ctx.style().window().group_border(2);
            ctx.style().window().group_border_color().set(white);

            ctx.style().tab().rounding(4);

        }
        return ctx;
    }
    public static NkColor createColor(MemoryStack stack,int r,int g,int b,int a){
        return NkColor.malloc(stack).r((byte) r).g((byte) g).b((byte) b).a((byte) a);
    }

    public static NkColor createColor(int r,int g,int b,int a){
        return NkColor.create().r((byte) r).g((byte) g).b((byte) b).a((byte) a);
    }
}
