#pragma once
#include "o2/Utils/Types/Ref.h"

namespace o2
{
    // -------------------------------------------------------------------
    // Basic events listener interface. Can be enabler or disabled, 
    // that setting up in constructor by global setting from events system
    // -------------------------------------------------------------------
    class IEventsListener: virtual public RefCounterable
    {
    public:
        // Default constructor. Takes global setting from events system and enables or disables this
        IEventsListener();

        // Virtual destructor
        virtual ~IEventsListener();

        // Returns is listener enabled
        bool IsListeningEvents() const;

        // Sets listener enabled to listen events
        void SetListeningEvents(bool isListening);

#if IS_EDITOR
        // Returns is editor mode
        bool IsEditorMode() const;

        // Sets editor mode
        void SetEditorMode(bool isEditorMode);
#endif

    protected:
        bool mEnabledListeningEvents = true;

#if IS_EDITOR
        bool mIsEditorMode = false;
#endif
    };
}
