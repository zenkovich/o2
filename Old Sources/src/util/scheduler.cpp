#include "scheduler.h"

#include "math\math.h"

OPEN_O2_NAMESPACE

bool Scheduler::Task::Execute()
{
	if (mVariableRepeatDelay)
	{
		mDelay = ((IRetCallback< float >*)mCallback)->InvokeRes();
	}
	else
	{
		mCallback->Invoke();
		mDelay = mRepeatDelay;
	}

	return mDelay >= 0.0f;
}


Scheduler::Scheduler()
{
	mCurrentTasks = &mTasks[0];
	mNextTasks = &mTasks[1];

	const int initTasksCount = 25;
	for (int i = 0; i < initTasksCount; i++)
		mUnusedTasks.Add(mnew Task);

	mLastTaskId = 0;
}

Scheduler::~Scheduler()
{
	ReleaseArray(mTasks[0]);
	ReleaseArray(mTasks[1]);
	ReleaseArray(mUnusedTasks);
}

void Scheduler::ProcessBeforeFrame(float dt)
{
	ProcessCurrentTasks(dt, ES_BEFORE_FRAME);
}

void Scheduler::ProcessAfterFrame(float dt)
{
	ProcessCurrentTasks(dt, ES_AFTER_FRAME);
	Swap(mCurrentTasks, mNextTasks);
	mNextTasks->Clear();
}

void Scheduler::ProcessCurrentTasks(float dt, ExecStage stage)
{
	foreach(*mCurrentTasks, task)
	{
		if ((*task)->mExecStage == stage)
		{
			(*task)->mDelay -= dt;

			if ((*task)->mDelay < 0)
			{
				if ((*task)->Execute())
				{
					mNextTasks->Add(*task);
				}
				else
				{
					mUnusedTasks.Add(*task);
				}
			}
			else
			{
				mNextTasks->Add(*task);
			}
		}
	}
}

int Scheduler::AddTask(ICallback* callback, float execDelay /*= 0.0f*/, ExecStage stage /*= ES_AFTER_FRAME*/)
{
	TasksArr* tasks = stage == ES_AFTER_FRAME ? mCurrentTasks:mNextTasks;
	Task* newTask = GetTask(callback, execDelay, -1, stage, false);
	tasks->Add(newTask);
	return newTask->mId;
}

int Scheduler::AddRepeatTask(ICallback* callback, float repeatDelay, float execDelay /*= 0.0f*/,
							 ExecStage stage /*= ES_AFTER_FRAME*/)
{
	TasksArr* tasks = stage == ES_AFTER_FRAME ? mCurrentTasks:mNextTasks;
	Task* newTask = GetTask(callback, execDelay, repeatDelay, stage, false);
	tasks->Add(newTask);
	return newTask->mId;
}

int Scheduler::AddRepeatTask(IRetCallback<float>* callback, float execDelay /*= 0.0f*/,
							 ExecStage stage /*= ES_AFTER_FRAME*/)
{
	TasksArr* tasks = stage == ES_AFTER_FRAME ? mCurrentTasks:mNextTasks;
	Task* newTask = GetTask(callback, execDelay, -1, stage, true);
	tasks->Add(newTask);
	return newTask->mId;
}

void Scheduler::ClearAllTasks()
{
	foreach(*mCurrentTasks, it)
		mUnusedTasks.Add(*it);

	foreach(*mNextTasks, it)
		mUnusedTasks.Add(*it);

	mCurrentTasks->Clear();
	mNextTasks->Clear();
}

void Scheduler::RemoveTask(int id)
{
	foreach(*mCurrentTasks, it)
	{
		if ((*it)->mId == id)
		{
			mCurrentTasks->Remove(it);
			mUnusedTasks.Add(*it);
			break;
		}
	}

	foreach(*mNextTasks, it)
	{
		if ((*it)->mId == id)
		{
			mNextTasks->Remove(it);
			mUnusedTasks.Add(*it);
			break;
		}
	}
}

Scheduler::Task* Scheduler::GetTask(ICallback* callback, float delay, float repeatDelay, ExecStage stage,
									bool variableRepeatDelay)
{
	Task* res;

	if (mUnusedTasks.Count() > 0)
	{
		res = mUnusedTasks.PopBack();
	}
	else
		res = mnew Task;

	res->mCallback = callback;
	res->mDelay = delay;
	res->mRepeatDelay = repeatDelay;
	res->mExecStage = stage;
	res->mVariableRepeatDelay = variableRepeatDelay;
	res->mId = mLastTaskId++;

	if (mLastTaskId == 9999999)
		mLastTaskId = 0;

	return res;
}

CLOSE_O2_NAMESPACE