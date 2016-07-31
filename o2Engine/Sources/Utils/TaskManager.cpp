#include "TaskManager.h"

#include "Utils/AnimationTask.h"
#include "Utils/Task.h"

namespace o2
{
	DECLARE_SINGLETON(TaskManager);

	void TaskManager::StopTask(int id)
	{
		for (auto task : mTasks)
		{
			if (task->mId == id)
			{
				delete task;
				return;
			}
		}
	}

	void TaskManager::StopAllTasks()
	{
		auto tasks = mTasks;
		for (auto task : tasks)
			delete task;
	}

	Task* TaskManager::FindTask(int id)
	{
		return mTasks.FindMatch([&](auto x) { return x->mId == id; });
	}

	void TaskManager::Run(const Function<void(float)>& update, const Function<bool()> isDone)
	{
		auto task = mnew FunctionalTask();
		task->update = update;
		task->isDone = isDone;
	}

	void TaskManager::Run(const Function<void(float)>& update, float time)
	{
		auto task = mnew FunctionalTimeTask(time);
		task->update = update;
	}

	void TaskManager::Invoke(const Function<void()> func, float delay)
	{
		auto task = mnew FunctionalDelayedTask(delay);
		task->doTask = func;
	}

	TaskManager::TaskManager():
		mLastTaskId(0)
	{}

	TaskManager::~TaskManager()
	{
		StopAllTasks();
	}

	void TaskManager::Update(float dt)
	{
		TasksVec doneTasks;
		for (auto task : mTasks)
		{
			task->Update(dt);
			if (task->IsDone())
				doneTasks.Add(task);
		}

		for (auto doneTask : doneTasks)
			delete doneTask;
	}

	void TaskManager::Play(const Animation& animation, float delay /*= 0.0f*/)
	{
		mnew AnimationTask(animation, delay);
	}
}