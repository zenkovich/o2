package com.test.testapplication;

// Wrapper for native library

import android.app.Activity;
import android.content.res.AssetManager;

public class NativeBridge {

    static {
        System.loadLibrary("app");
    }

    /**
     * @param width the current view width
     * @param height the current view height
     */
    public static native void init(Activity activity, AssetManager assetManager, String dataPath, int width, int height);
    public static native void step();
}
