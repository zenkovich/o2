#pragma once

#include <vector>
#include "public.h"
#include "callback.h"
#include "smart_ptrs.h"

OPEN_O2_NAMESPACE

/** Scheduler class. Processing tasks at begin and end frame. */
class Scheduler
{
	friend class Application;
	friend class ApplicationBaseInterface;

public:
	enum ExecStage { ES_BEFORE_FRAME = 0, ES_AFTER_FRAME };

protected:
	/** Task container struct. */
	struct Task
	{
		float      mDelay;               /** Delay to next execution. */
		float      mRepeatDelay;         /** Repeat delay. If no repeat == -1. */
		ExecStage  mExecStage;           /** Execution stage. */
		ICallback* mCallback;            /** Execution callback. */
		bool       mVariableRepeatDelay; /** True, when repeat delay will be get from callback result. */
		int        mId;                  /** Task id. */

		/** Executes the callback and returning true, when task must be repeated. */
		bool Execute();
	};
	typedef Array<Task*> TasksArr;

	TasksArr  mTasks[2];     /** Tasks vectors. One for current tasks, other for next tasks. */
	TasksArr* mCurrentTasks; /** Current tasks vector ptr. */
	TasksArr* mNextTasks;    /** Next tasks vector ptr. */
	TasksArr  mUnusedTasks;  /** Unused tasks vector. When needs some task object, first will take from here. */
	int       mLastTaskId;   /** Last task id. */

public:
	/** ctor. */
	Scheduler();

	/** dtor. */
	~Scheduler();

	/** Adding once time execution callback. Return id of task. */
	int AddTask(ICallback* callback, float execDelay = 0.0f, ExecStage stage = ES_AFTER_FRAME);

	/** Adding repeating callback, repeats with repeatDelay seconds and never stops. Returns task id. */
	int AddRepeatTask(ICallback* callback, float repeatDelay, float execDelay = 0.0f, ExecStage stage = ES_AFTER_FRAME);

	/** Adding repeating callback, repeat delay gets from callback result. Returns task id. */
	int AddRepeatTask(IRetCallback<float>* callback, float execDelay = 0.0f, ExecStage stage = ES_AFTER_FRAME);

	/** Removes task by id, task will never executed. */
	void RemoveTask(int id);

	/** Removes all tasks. */
	void ClearAllTasks();

protected:
	/** Processing tasks before frame. */
	void ProcessBeforeFrame(float dt);

	/** Processing tasks after frame. */
	void ProcessAfterFrame(float dt);

	/** Processing mCurrentTasks with specified dt and stage. */
	void ProcessCurrentTasks(float dt, ExecStage stage);

	/** Returns task sample filled with specified parameters. */
	Task* GetTask(ICallback* callback, float delay, float repeatDelay, ExecStage stage, bool variableRepeatDelay);
};

CLOSE_O2_NAMESPACE