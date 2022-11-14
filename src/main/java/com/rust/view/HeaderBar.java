package com.rust.view;

import com.rust.view.window.style.Style;
import org.lwjgl.nuklear.NkCommandBuffer;
import org.lwjgl.nuklear.NkContext;
import org.lwjgl.nuklear.NkRect;
import org.lwjgl.system.MemoryStack;

import static org.lwjgl.nuklear.Nuklear.*;

public class HeaderBar {
    public Integer headerWidth, headerHeight;

    public void layout(NkContext ctx){
        try (MemoryStack stack = MemoryStack.stackPush()){
            NkRect rect = NkRect.malloc(stack);
            rect.set(0,0,headerWidth,200);
            String title = "的";
            if(nk_begin(ctx,title,rect,0)){
                NkCommandBuffer canvas = nk_window_get_canvas(ctx);
                NkRect r = NkRect.malloc(stack);
                r.set(0,0,headerHeight,headerHeight);
                nk_fill_rect(canvas,r,2,Style.blue1);
                nk_end(ctx);
            }
        }
    }
}
