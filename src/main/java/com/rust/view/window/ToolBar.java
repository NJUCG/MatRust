package com.rust.view.window;

import org.lwjgl.nuklear.NkColor;
import org.lwjgl.nuklear.NkContext;
import org.lwjgl.nuklear.NkRect;
import org.lwjgl.nuklear.NkVec2;
import org.lwjgl.system.MemoryStack;

import static org.lwjgl.nuklear.Nuklear.*;
import static org.lwjgl.nuklear.Nuklear.nk_end;

public class ToolBar {

    public void layout(NkContext ctx,int x,int y){
        try(MemoryStack stack = MemoryStack.stackPush()){
            NkRect rect = NkRect.malloc(stack);
            if (nk_begin(
                    ctx,
                    "",
                    nk_rect(x, y, 230, 250, rect),
                    NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE
            )) {
                nk_layout_row_static(ctx, 30, 80, 1);
                if (nk_button_label(ctx, "button1")) {
                    System.out.println("button1 pressed");
                }
                nk_layout_row_dynamic(ctx, 30, 2);
                nk_layout_row_end(ctx);
                nk_layout_row_dynamic(ctx, 20, 1);
                nk_label(ctx, "background:", NK_TEXT_LEFT);
                nk_layout_row_dynamic(ctx, 25, 1);
            }
            nk_end(ctx);
        }
    }
}
