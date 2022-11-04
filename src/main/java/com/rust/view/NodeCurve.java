package com.rust.view;

import org.lwjgl.nuklear.NkVec2;

public class NodeCurve {
    public NkVec2 start = NkVec2.create();
    public NkVec2 stop = NkVec2.create();
    public int stopNodeId;
    public int stopSlotId;
    public int startNodeId;
    public int startSlotId;
    public boolean hasMe(int nodeId,int slotId){
        return startNodeId == nodeId && startSlotId == slotId;
    }
    public boolean isMe(int nodeId,int slotId){
        return stopNodeId == nodeId && stopSlotId == slotId;
    }
}
