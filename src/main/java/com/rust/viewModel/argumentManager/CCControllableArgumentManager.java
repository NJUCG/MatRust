package com.rust.viewModel.argumentManager;

public class CCControllableArgumentManager extends ControllableArgumentManager{
    @Override
    public String getName() {
        return "Chlorine";
    }

    @Override
    protected double caster(double v) {
        return v/90;
    }
}
