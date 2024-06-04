#include "o2/stdafx.h"
#include "Task.h"

#include "o2/Utils/Tasks/TaskManager.h"

namespace o2
{
    Task::Task(RefCounter* refCounter):
        RefCounterable(refCounter)
    {
        mId = o2Tasks.mLastTaskId++;
        o2Tasks.mTasks.Add(Ref(this));
    }

    Task::Task(RefCounter* refCounter, const Task& other):
		RefCounterable(refCounter)
    {
        mId = o2Tasks.mLastTaskId++;
        o2Tasks.mTasks.Add(Ref(this));
    }

    Task::~Task()
    {}

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

	FunctionalTask::FunctionalTask(RefCounter* refCounter):
		Task(refCounter)
	{}

	void FunctionalTask::Update(float dt)
    {
        update(dt);
    }

    bool FunctionalTask::IsDone() const
    {
        return isDone();
    }

    TimeTask::TimeTask(RefCounter* refCounter, float time /*= 1.0f*/):
        Task(refCounter), mRemainingTime(time)
    {}

    void TimeTask::Update(float dt)
    {
        mRemainingTime -= dt;
    }

    bool TimeTask::IsDone() const
    {
        return mRemainingTime < 0;
    }

    FunctionalTimeTask::FunctionalTimeTask(RefCounter* refCounter, float time /*= 1.0f*/):
        TimeTask(refCounter, time)
    {}

    void FunctionalTimeTask::Update(float dt)
    {
        update(dt);
    }

    DelayedTask::DelayedTask(RefCounter* refCounter, float delay /*= 0.0f*/):
        Task(refCounter), mRemainingToCallTime(delay)
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

    FunctionalDelayedTask::FunctionalDelayedTask(RefCounter* refCounter, float delay /*= 0.0f*/):
        DelayedTask(refCounter, delay)
    {}

    void FunctionalDelayedTask::DoTask()
    {
        doTask();
    }

}
