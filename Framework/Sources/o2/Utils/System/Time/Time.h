#pragma once

#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/System/Time/TimeStamp.h"
#include "o2/Utils/Types/CommonTypes.h"

// Time stuff access macros
#define o2Time o2::Time::Instance()

namespace o2
{
    // -------------
    // Timing system
    // -------------
    class Time: public Singleton<Time>
    {
    public:
        // Destructor
        ~Time();

        // Returns application working time, in seconds
        float GetApplicationTime() const;

        // Returns local time, in seconds
        float GetLocalTime() const;

        // Resetting local time to zero
        void ResetLocalTime() const;

        // Resetting local time to value
        void SetLocalTime(float time) const;

        // Returns current frame index
        int GetCurrentFrame() const;

        // Returns frames delta time
        float GetDeltaTime() const;

        // Returns average frames per second for 0.3 seconds
        float GetFPS() const;

        // Return current system time
        TimeStamp CurrentTime() const;

        // Update parameters
        void Update(float dt);

    public:
        friend class Application;
        friend class BaseApplication;

        float  mApplicationTime; // Application working time, seconds
        float  mLocalTime;       // Local timer working, seconds. Controlling by user
        UInt64 mCurrentFrame;    // Current frame index
        float  mDeltaTime;       // Frame delta time
        float  mFPS;             // Average frames per second

        float mFPSSum;              // Summary of fps
        float mFramesSum;           // Frames summary
        float mLastFPSCheckingTime; // Last average fps checking time

    protected:
        // Default constructor
        Time();
    };
}
