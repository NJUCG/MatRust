package com.rust.viewModel.argumentManager;

import java.awt.event.AdjustmentEvent;

public class RHControllableArgumentManager extends ControllableArgumentManager{

    @Override
    public String getName() {
        return "RH";
    }

    @Override
    protected double caster(double v) {
        return v / 90;
    }
}
