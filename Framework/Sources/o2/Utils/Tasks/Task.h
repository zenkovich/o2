#pragma once

#include "o2/Utils/Function/Function.h"

namespace o2
{
    // --------------------
    // Basic task interface
    // --------------------
    class Task
    {
    public:
        // Default constructor. Registering himself in tasks manager and gets id
        Task();

        // Copy-constructor. Registering himself in tasks manager and gets id
        Task(const Task& other);

        // Destructor. Removes from tasks manager
        virtual ~Task();

        // Updates task
        virtual void Update(float dt);

        // Checks task for complete
        virtual bool IsDone() const;

        // Returns task identificator
        int ID() const;

    protected:
        int mId; // Task identificator

        friend class TaskManager;
    };

    // ---------------
    // Functional task
    // ---------------
    class FunctionalTask: public Task
    {
    public:
        Function<void(float)> update;
        Function<bool()> isDone;

        void Update(float dt);
        bool IsDone() const;
    };

    // ----------
    // Timed task
    // ----------
    class TimeTask: public Task
    {
    public:
        TimeTask(float time = 1.0f);

        void Update(float dt);
        bool IsDone() const;

    protected:
        float mRemainingTime;
    };

    // ---------------------
    // Functional timed task
    // ---------------------
    class FunctionalTimeTask: public TimeTask
    {
    public:
        Function<void(float)> update;

        FunctionalTimeTask(float time = 1.0f);
        void Update(float dt);
    };

    // ------------
    // Delayed task
    // ------------
    class DelayedTask: Task
    {
    public:
        DelayedTask(float delay = 0.0f);

        void Update(float dt);
        bool IsDone() const;

    protected:
        float mRemainingToCallTime;

    protected:
        virtual void DoTask();
    };

    // -----------------------
    // Functional delayed task
    // -----------------------
    class FunctionalDelayedTask: DelayedTask
    {
    public:
        Function<void()> doTask;

        FunctionalDelayedTask(float delay = 0.0f);

    protected:
        void DoTask();
    };
}
