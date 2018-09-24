#include <jni.h>

#include "O2.h"
#include "TestApplication.h"
#include <android/asset_manager_jni.h>
#include <GLES2/gl2.h>
#include <Application/Input.h>

TestApplication* app = nullptr;
Vec2I screenSize;

extern "C"
JNIEXPORT void JNICALL Java_com_test_testapplication_NativeBridge_init(JNIEnv *env, jobject obj,
                                                                       jobject crossActivity, jobject assetManager,
                                                                       jstring dataPath, jint width, jint height)
{
    INITIALIZE_O2;

    screenSize = Vec2I(width, height);

    AAssetManager* nativeAssetManager = AAssetManager_fromJava(env, assetManager);

    app = mnew TestApplication();
    app->Initialize(env, env->NewGlobalRef(obj), nativeAssetManager, env->GetStringUTFChars(dataPath, NULL), screenSize);
    app->Launch();
}

extern "C"
JNIEXPORT void JNICALL Java_com_test_testapplication_NativeBridge_step(JNIEnv * env, jobject obj)
{
    app->Update();
}

extern "C"
JNIEXPORT void JNICALL Java_com_test_testapplication_NativeBridge_TouchDown(JNIEnv *env, jclass type, jint id, jint x, jint y)
{
    o2Input.OnCursorPressed(Vec2F(x - screenSize.x/2, screenSize.y/2 - y), id);
}

extern "C"
JNIEXPORT void JNICALL Java_com_test_testapplication_NativeBridge_TouchMove(JNIEnv *env, jclass type, jint id, jint x, jint y)
{
    o2Input.OnCursorMoved(Vec2F(x - screenSize.x/2, screenSize.y/2 - y), id);
}

extern "C"
JNIEXPORT void JNICALL Java_com_test_testapplication_NativeBridge_TouchUp(JNIEnv *env, jclass type, jint id, jint x, jint y)
{
    o2Input.OnCursorMoved(Vec2F(x - screenSize.x/2, screenSize.y/2 - y), id);
    o2Input.OnCursorReleased(id);
}