#pragma once

#include "o2/Utils/Function/Function.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Utils/Types/Ref.h"

// Task manager access macros
#define o2Tasks o2::TaskManager::Instance()

namespace o2
{
    class Task;
    FORWARD_REF(Task);

    // -----------------------
    // Tasks manager singleton
    // -----------------------
    class TaskManager: public Singleton<TaskManager>, public RefCounterable
    {
    public:
        // Default constructor
        TaskManager();

        // Destructor. Destroys all tasks
        ~TaskManager();

        // Stops task with specified id
        void StopTask(int id);

        // Stops all tasks
        void StopAllTasks();

        // Returns task
        Ref<Task> FindTask(int id);

        // Runs new functional task
        void Run(const Function<void(float)>& update, const Function<bool()> isDone);

        // Runs timed task
        void Run(const Function<void(float)>& update, float time);

        // Called function after delay
        void Invoke(const Function<void()> func, float delay = 0.0f);

        // Updates tasks and checking for done
        void Update(float dt);

    protected:
        Vector<Ref<Task>> mTasks; // All tasks array

        int mLastTaskId = 0; // Last given task id
        
    protected:
        friend class Task;
        friend class Application;
    };
}
