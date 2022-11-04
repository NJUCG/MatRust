package com.rust.viewModel.argumentManager;

public class SCControllableArgumentManager extends ControllableArgumentManager{
    @Override
    public String getName() {
        return "Sulfur";
    }

    @Override
    protected double caster(double v) {
        return  v/90;
    }
}
