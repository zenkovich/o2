#include <jni.h>

#include "TestApplication.h"

extern "C" {
JNIEXPORT void JNICALL Java_com_test_testapplication_NativeBridge_init(JNIEnv * env, jobject obj,  jint width, jint height);
JNIEXPORT void JNICALL Java_com_test_testapplication_NativeBridge_step(JNIEnv * env, jobject obj);
};

TestApplication* app = nullptr;

JNIEXPORT void JNICALL Java_com_test_testapplication_NativeBridge_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    app = mnew TestApplication();
    //setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_test_testapplication_NativeBridge_step(JNIEnv * env, jobject obj)
{
    //renderFrame();
}
