package com.rust.viewModel.argumentManager;

public class OCControllableArgumentManager extends ControllableArgumentManager{
    @Override
    public String getName() {
        return "Oxygen";
    }

    @Override
    protected double caster(double v) {
        return v/90;
    }
}
