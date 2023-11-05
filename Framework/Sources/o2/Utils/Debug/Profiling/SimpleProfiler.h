#pragma once
#include <unordered_map>

#include "o2/Utils/Types/Containers/Pair.h"
#include "o2/Utils/System/Time/Timer.h"

namespace o2
{    
    class SimpleProfiler
    {
    public:
        struct ScopeSampler
        {
            const char* id;
            float begin, end;

            ScopeSampler(const char* id): 
                id(id), begin(SimpleProfiler::GetProfileTime())
            {}

            ~ScopeSampler()
            {
                end = SimpleProfiler::GetProfileTime();
                SimpleProfiler::Sample(id, begin, end);
            }
        };

        struct ZoneSampler
        {
            const char* id;
            float begin, end;

            ZoneSampler(const char* id) :
                id(id), begin(SimpleProfiler::GetProfileTime())
            {}

            void End()
            {
                end = SimpleProfiler::GetProfileTime();
                SimpleProfiler::Sample(id, begin, end);
            }
        };

        struct SampleInterval
        {
            const char* id;
            float begin, end;

            SampleInterval(const char* id, float begin, float end) :
                id(id), begin(begin), end(end)
            {}
        };

    public:
        static void Reset();
        static void Flush();

        static void DumpLog();

        static void Sample(const char* id, float start, float end);

        static const Vector<SampleInterval>& GetSamples();
        static const std::unordered_map<const char*, Pair<float, int>>& GetAccumulatedSamples();
        static int GetAccumulatedSamplesCount();

        static float GetProfileTime();

    private:
        static Timer mTimer;
        static Vector<SampleInterval> mSamples;
        static std::unordered_map<const char*, Pair<float, int>> mAccumulatedSamples;
        static int mAccumulatedSamplesCount;
    };

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#if defined(O2_PROFILE_STATS)
#define PROFILE_SAMPLE_FUNC() o2::SimpleProfiler::ScopeSampler __scope_sampler(__PRETTY_FUNCTION__)
#define PROFILE_SAMPLE(id) o2::SimpleProfiler::ScopeSampler __scope_sampler(id)
#else
#define PROFILE_SAMPLE_FUNC()
#define PROFILE_SAMPLE(id)
#endif

}
