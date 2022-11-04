package com.rust.util;

import java.util.ArrayList;

public class Event {
    protected ArrayList<EventHandler> handlers = new ArrayList<>();
    public void register(EventHandler handler){
        if (!handlers.contains(handler)) {
            handlers.add(handler);
        }
    }
    public void unregister(EventHandler handler){
        handlers.remove(handler);
    }
    public void trigger(){
        for (EventHandler handler:handlers
             ) {
            handler.trigger();
        }
    }
}
