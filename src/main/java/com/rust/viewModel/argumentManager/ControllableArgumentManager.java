package com.rust.viewModel.argumentManager;

import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;

public class ControllableArgumentManager implements AdjustmentListener {
    double v;

    public double value() {
        return v;
    }

    public String getName(){
        return "";
    }

    public ControllableArgumentManager(){

    }

    @Override
    public void adjustmentValueChanged(AdjustmentEvent e) {
        v = caster(e.getValue());
    }

    protected double caster(double v){
        return 0;
    }
}
