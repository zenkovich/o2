package com.test.testapplication;

// Wrapper for native library

public class NativeBridge {

    static {
        System.loadLibrary("app");
    }

    /**
     * @param width the current view width
     * @param height the current view height
     */
    public static native void init(int width, int height);
    public static native void step();
}
