package com.rust.view;

import com.rust.view.window.Node;

public interface ILinkingResponder {
    void startLinking(Node node,int slot);
    void linkingTo(Node node, Integer slot);
    void linkingToByIndex(Node node,Integer index);
    void delSlot(Node node,Integer slot,boolean is_start);
}
