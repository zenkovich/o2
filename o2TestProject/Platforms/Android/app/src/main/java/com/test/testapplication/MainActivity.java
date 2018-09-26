package com.test.testapplication;

import android.app.Activity;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;

public class MainActivity extends Activity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("freetype2");
        System.loadLibrary("app");
    }

    GLView mView;

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().requestFeature(Window.FEATURE_NO_TITLE);
        mView = new GLView(getApplication());
        setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }

    @Override public boolean onTouchEvent(MotionEvent event) {
        int actionIndex = event.getActionIndex();
        int actionId = event.getPointerId(actionIndex);
        int actionMasked = event.getActionMasked();
        switch (actionMasked){
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN:{
                NativeBridge.TouchDown(actionIndex, (int)event.getX(actionIndex), (int)event.getY(actionIndex));
                return true;
            }
            case MotionEvent.ACTION_MOVE:{
                for(int i = 0; i < event.getPointerCount(); i++){
                    NativeBridge.TouchMove(event.getPointerId(i), (int)event.getX(actionIndex), (int)event.getY(actionIndex));
                }
                return true;
            }
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP: {
                NativeBridge.TouchUp(actionIndex, (int)event.getX(actionIndex), (int)event.getY(actionIndex));
                return true;
            }
        }
        return super.onTouchEvent(event);
    }
}
