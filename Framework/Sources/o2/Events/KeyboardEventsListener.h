#pragma once

#include "o2/Application/Input.h"
#include "o2/Events/IEventsListener.h"

namespace o2
{
    // ------------------------
    // Keyboard events listener
    // ------------------------
    class KeyboardEventsListener: public IEventsListener
    {
    public:
        // Default constructor
        KeyboardEventsListener();

        // Virtual destructor
        virtual ~KeyboardEventsListener();

        void PostRefConstruct();

    protected:
        // Called when key was pressed
        virtual void OnKeyPressed(const Input::Key& key);

        // Called when key was released
        virtual void OnKeyReleased(const Input::Key& key);

        // Called when key stay down during frame
        virtual void OnKeyStayDown(const Input::Key& key);

        friend class EventSystem;
    };
}
