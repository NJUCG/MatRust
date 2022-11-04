package com.rust.viewModel.argumentManager;


public class TemperatureArgumentManager extends ControllableArgumentManager {
    @Override
    public String getName() {
        return "T";
    }

    @Override
    protected double caster(double v) {
        return  (v/90) * 100 + 273;
    }
}
