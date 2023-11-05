#pragma once

#ifdef PLATFORM_ANDROID

#include <jni.h>
#include <android/asset_manager.h>
#include "Application/Android/VKCodes.h"

namespace o2
{
    // -------------------------------
    // Android application base fields
    // -------------------------------
    class ApplicationBase
    {
    protected:
        Vec2I mResolution;

        JavaVM* mJVM;
        jobject mActivity;
        AAssetManager* mAssetManager;
        String mDataPath;

    public:
        // Returns android Java virtual machine
        JavaVM* GetJVM() const;

        // Returns android activity
        const jobject* GetActivity() const;

        // Returns android asset manager
        AAssetManager* GetAssetManager() const;

        // Returns android data path
        String GetDataPath() const;
    };
}

#endif // PLATFORM_WINDOWS
