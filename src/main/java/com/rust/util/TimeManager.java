package com.rust.util;

public class TimeManager
{
    public static TimeManager shared = new TimeManager();
    // timing
    public float deltaTime = 0.0f;	// time between current frame and last frame
    public float lastFrame = 0.0f;
}
