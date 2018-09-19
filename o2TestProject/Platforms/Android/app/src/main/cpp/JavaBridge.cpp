#include <jni.h>

#include "O2.h"
#include "TestApplication.h"
#include <android/asset_manager_jni.h>
#include <GLES2/gl2.h>

extern "C" {
JNIEXPORT void JNICALL Java_com_test_testapplication_NativeBridge_init(JNIEnv *env, jobject obj,
                                                                       jobject crossActivity, jobject assetManager,
                                                                       jstring dataPath, jint width, jint height);

JNIEXPORT void JNICALL Java_com_test_testapplication_NativeBridge_step(JNIEnv * env, jobject obj);
};

TestApplication* app = nullptr;

JNIEXPORT void JNICALL Java_com_test_testapplication_NativeBridge_init(JNIEnv *env, jobject obj,
                                                                       jobject crossActivity, jobject assetManager,
                                                                       jstring dataPath, jint width, jint height)
{
    INITIALIZE_O2;

    AAssetManager* nativeAssetManager = AAssetManager_fromJava(env, assetManager);

    app = mnew TestApplication();
    app->Initialize(env, env->NewGlobalRef(obj), nativeAssetManager, env->GetStringUTFChars(dataPath, NULL), Vec2I(width, height));
    app->Launch();
}

JNIEXPORT void JNICALL Java_com_test_testapplication_NativeBridge_step(JNIEnv * env, jobject obj)
{
    app->Update();
}
