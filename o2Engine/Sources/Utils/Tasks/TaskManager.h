#pragma once

#include "Utils/Types/Containers/Vector.h"
#include "Utils/Singleton.h"
#include "Utils/Delegates.h"

// Task manager access macros
#define o2Tasks o2::TaskManager::Instance()

namespace o2
{
	class Task;
	class Animation;

	// -----------------------
	// Tasks manager singleton
	// -----------------------
	class TaskManager: public Singleton<TaskManager>
	{
	public:
		typedef Vector<Task*> TasksVec;

	public:
		// Stops task with specified id
		void StopTask(int id);

		// Stops all tasks
		void StopAllTasks();

		// Returns task
		Task* FindTask(int id);

		// Runs new functional task
		void Run(const Function<void(float)>& update, const Function<bool()> isDone);

		// Runs timed task
		void Run(const Function<void(float)>& update, float time);

		// It is called function after delay
		void Invoke(const Function<void()> func, float delay);

		// Updates tasks and checking for done
		void Update(float dt);

		// Plays animation task
		void Play(const Animation& animation, float delay = 0.0f);

	protected:
		TasksVec mTasks;      // All tasks array
		int      mLastTaskId; // Last given task id
		
	protected:
		// Default constructor
		TaskManager();

		// Destructor. Destroys all tasks
		~TaskManager();

		friend class Task;
		friend class Application;
	};
}
