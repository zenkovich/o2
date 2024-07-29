#include "o2/stdafx.h"
#include "TaskManager.h"

#include "o2/Utils/Tasks/Task.h"

namespace o2
{
    DECLARE_SINGLETON(TaskManager);
    FORWARD_REF_IMPL(Task);

    void TaskManager::StopTask(int id)
    {
        for (auto& task : mTasks)
        {
            if (task->mId == id)
                return;
        }
    }

    void TaskManager::StopAllTasks()
    {
        mTasks.Clear();
    }

    Ref<Task> TaskManager::FindTask(int id)
    {
        return mTasks.FindOrDefault([&](auto x) { return x->mId == id; });
    }

    void TaskManager::Run(const Function<void(float)>& update, const Function<bool()> isDone)
    {
        auto task = mmake<FunctionalTask>();
        task->update = update;
        task->isDone = isDone;
    }

    void TaskManager::Run(const Function<void(float)>& update, float time)
    {
        auto task = mmake<FunctionalTimeTask>(time);
        task->update = update;
    }

    void TaskManager::Invoke(const Function<void()> func, float delay /*= 0*/)
    {
        auto task = mmake<FunctionalDelayedTask>(delay);
        task->doTask = func;
    }

    TaskManager::TaskManager(RefCounter* refCounter):
        Singleton<TaskManager>(refCounter), mLastTaskId(0)
    {}

    TaskManager::~TaskManager()
    {
        StopAllTasks();
    }

    void TaskManager::Update(float dt)
    {
        PROFILE_SAMPLE_FUNC();

        Vector<Ref<Task>> doneTasks;
        Vector<Ref<Task>> tasks = mTasks;
        for (auto& task : tasks)
        {
            task->Update(dt);

            if (task->IsDone())
                doneTasks.Add(task);
        }

        mTasks.Remove(doneTasks);
    }
}
