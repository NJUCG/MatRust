package com.rust.view.window.style;

import org.lwjgl.nuklear.*;
import org.lwjgl.system.MemoryStack;

import java.nio.ByteBuffer;

import static org.lwjgl.nuklear.Nuklear.*;

public class Style {
    public static Style style = new Style();
    private Style(){

    }
    public NkContext statusStyle(NkContext ctx){
        try (MemoryStack stack = MemoryStack.stackPush()){
            NkColor gray50 = createColor(stack,0,0,0,128);
            NkColor transparent = createColor(stack,0,0,0,0);
            NkColor gray75 = createColor(stack,0,0,0,192);
            NkColor white = createColor(stack, 255, 255, 255, 255);
            NkColor black = createColor(stack, 0, 0, 0, 255);
            NkColor grey01 = createColor(stack, 45, 45, 45, 255);
            NkColor grey02 = createColor(stack, 70, 70, 70, 255);
            NkColor grey03 = createColor(stack, 120, 120, 120, 255);
            NkColor grey04 = createColor(stack, 140, 140, 140, 255);
            NkColor grey05 = createColor(stack, 150, 150, 150, 255);
            NkColor grey06 = createColor(stack, 160, 160, 160, 255);
            NkColor grey07 = createColor(stack, 170, 170, 170, 255);
            NkColor grey08 = createColor(stack, 180, 180, 180, 255);
            NkColor grey09 = createColor(stack, 185, 185, 185, 255);
            NkColor grey10 = createColor(stack, 190, 190, 190, 255);
            NkColor grey11 = createColor(stack, 200, 200, 200, 255);
            NkColor grey12 = createColor(stack, 240, 240, 240, 255);
            NkColor blue1 = createColor(stack, 80, 80, 200, 255);
            NkColor blue2 = createColor(stack, 128, 196, 255, 255);
            NkColor blue3 = createColor(stack, 64, 196, 255, 255);
            NkColor red = createColor(stack, 255, 0, 0, 255);
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
            NkColor gray50 = createColor(stack,0,0,0,128);
            NkColor transparent = createColor(stack,0,0,0,0);
            NkColor gray75 = createColor(stack,0,0,0,192);
            NkColor white = createColor(stack, 255, 255, 255, 255);
            NkColor black = createColor(stack, 0, 0, 0, 255);
            NkColor grey00 = createColor(stack, 20, 20, 20, 255);
            NkColor grey01 = createColor(stack, 45, 45, 45, 255);
            NkColor grey02 = createColor(stack, 70, 70, 70, 255);
            NkColor grey03 = createColor(stack, 120, 120, 120, 255);
            NkColor grey04 = createColor(stack, 140, 140, 140, 255);
            NkColor grey05 = createColor(stack, 150, 150, 150, 255);
            NkColor grey06 = createColor(stack, 160, 160, 160, 255);
            NkColor grey07 = createColor(stack, 170, 170, 170, 255);
            NkColor grey08 = createColor(stack, 180, 180, 180, 255);
            NkColor grey09 = createColor(stack, 185, 185, 185, 255);
            NkColor grey10 = createColor(stack, 190, 190, 190, 255);
            NkColor grey11 = createColor(stack, 200, 200, 200, 255);
            NkColor grey12 = createColor(stack, 240, 240, 240, 255);
            NkColor blue1 = createColor(stack, 80, 80, 200, 255);
            NkColor blue2 = createColor(stack, 128, 196, 255, 255);
            NkColor blue3 = createColor(stack, 64, 196, 255, 255);
            NkColor red = createColor(stack, 255, 0, 0, 255);
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
        }
        return ctx;
    }
    public NkColor createColor(MemoryStack stack,int r,int g,int b,int a){
        return NkColor.malloc(stack).r((byte) r).g((byte) g).b((byte) b).a((byte) a);
    }

}
