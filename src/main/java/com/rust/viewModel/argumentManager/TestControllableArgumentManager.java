package com.rust.viewModel.argumentManager;

public class TestControllableArgumentManager extends ControllableArgumentManager{
    @Override
    public String getName() {
        return "Test";
    }


    @Override
    protected double caster(double v) {
        return v;
    }
}
