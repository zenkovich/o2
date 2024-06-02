#include "o2/stdafx.h"
#include "SimpleProfiler.h"

namespace o2
{
    void SimpleProfiler::Reset()
    {
        mTimer.Reset();

        mSamples.Clear();
        mAccumulatedSamples.clear();

        mAccumulatedSamplesCount = 0;
    }

    void SimpleProfiler::Flush()
    {
        for (auto& sample : mSamples)
        {
            auto& accumulated = mAccumulatedSamples[sample.id];
            accumulated.first += sample.end - sample.begin;
            accumulated.second++;
        }

        mSamples.Clear();
        mAccumulatedSamplesCount++;
    }

    void SimpleProfiler::Sample(const char* id, float start, float end)
    {
        mSamples.Add({ id, start, end });
    }

    const Vector<SimpleProfiler::SampleInterval>& SimpleProfiler::GetSamples()
    {
        return mSamples;
    }

    const std::unordered_map<const char*, Pair<float, int>>& SimpleProfiler::GetAccumulatedSamples()
    {
        return mAccumulatedSamples;
    }

    int SimpleProfiler::GetAccumulatedSamplesCount()
    {
        return mAccumulatedSamplesCount;
    }

    void SimpleProfiler::DumpLog()
    {
        o2Debug.Log("---- Simple profiler dump for " + (String)mAccumulatedSamplesCount + " frames:");

        static Vector<Pair<const char*, Pair<float, int>>> sortedSamples;
        sortedSamples.Clear();

        float invMS = 1000.0f / (float)mAccumulatedSamplesCount;
        for (auto& kv : mAccumulatedSamples)
            sortedSamples.Add({ kv.first, { kv.second.first * invMS, kv.second.second } });

        sortedSamples.Sort([](auto& a, auto& b) { return a.second > b.second; });

        float invFramesCount = 1.0f / (float)mAccumulatedSamplesCount;

        for (auto& kv : sortedSamples)
        {
            o2Debug.Log("   %f ms (%.2f): %s", kv.second.first, (float)kv.second.second*invFramesCount, kv.first);
        }
    }

    float SimpleProfiler::GetProfileTime()
    {
        return mTimer.GetTime();
    }

    Timer SimpleProfiler::mTimer;
    Vector<SimpleProfiler::SampleInterval> SimpleProfiler::mSamples;
    std::unordered_map<const char*, Pair<float, int>> SimpleProfiler::mAccumulatedSamples;
    int SimpleProfiler::mAccumulatedSamplesCount;
}
