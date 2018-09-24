package com.test.testapplication;

// Wrapper for native library

import android.app.Activity;
import android.content.res.AssetManager;

public class NativeBridge {

    static {
        System.loadLibrary("app");
    }

    public static native void init(Activity activity, AssetManager assetManager, String dataPath, int width, int height);
    public static native void step();
    public static native void TouchDown(int id, int x, int y);
    public static native void TouchMove(int id, int x, int y);
    public static native void TouchUp(int id, int x, int y);
}
