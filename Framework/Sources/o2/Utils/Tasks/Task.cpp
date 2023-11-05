#include "o2/stdafx.h"
#include "Task.h"

#include "o2/Utils/Tasks/TaskManager.h"

namespace o2
{
    Task::Task()
    {
        mId = o2Tasks.mLastTaskId++;
        o2Tasks.mTasks.Add(this);
    }

    Task::Task(const Task& other)
    {
        mId = o2Tasks.mLastTaskId++;
        o2Tasks.mTasks.Add(this);
    }

    Task::~Task()
    {
        o2Tasks.mTasks.Remove(this);
    }

    void Task::Update(float dt)
    {}

    bool Task::IsDone() const
    {
        return true;
    }

    int Task::ID() const
    {
        return mId;
    }

    void FunctionalTask::Update(float dt)
    {
        update(dt);
    }

    bool FunctionalTask::IsDone() const
    {
        return isDone();
    }

    TimeTask::TimeTask(float time /*= 1.0f*/):
        Task(), mRemainingTime(time)
    {}

    void TimeTask::Update(float dt)
    {
        mRemainingTime -= dt;
    }

    bool TimeTask::IsDone() const
    {
        return mRemainingTime < 0;
    }

    FunctionalTimeTask::FunctionalTimeTask(float time /*= 1.0f*/):
        TimeTask(time)
    {}

    void FunctionalTimeTask::Update(float dt)
    {
        update(dt);
    }

    DelayedTask::DelayedTask(float delay /*= 0.0f*/):
        Task(), mRemainingToCallTime(delay)
    {}

    void DelayedTask::Update(float dt)
    {
        mRemainingToCallTime -= dt;
        if (mRemainingToCallTime < 0)
            DoTask();
    }

    bool DelayedTask::IsDone() const
    {
        return mRemainingToCallTime < 0;
    }

    void DelayedTask::DoTask()
    {}

    FunctionalDelayedTask::FunctionalDelayedTask(float delay /*= 0.0f*/):
        DelayedTask(delay)
    {}

    void FunctionalDelayedTask::DoTask()
    {
        doTask();
    }

}
