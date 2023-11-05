#pragma once

#include "o2/Events/IEventsListener.h"
#include "o2/Events/KeyboardEventsListener.h"
#include "o2/Utils/System/ShortcutKeys.h"

namespace o2
{
    // ----------------------------------------------------------------------------------------
    // Shortcut keys listener. Calls event when hit shortcut and this listener has max priority
    // ----------------------------------------------------------------------------------------
    class ShortcutKeysListener: public IEventsListener
    {
    public:
        // Virtual destructor. Unregistering from manager
        virtual ~ShortcutKeysListener();

        // Sets maximum priority for this listener
        void SetMaxPriority();

        // Sets minimal priority
        void SetMinPriority();

        // Sets shortcut keys and registering in manager
        void SetShortcut(const ShortcutKeys& shortcut);

        // Returns shortcut keys
        const ShortcutKeys& GetShortcut() const;

        // Sets enable. Not enabled listeners doesn't react on shortcut
        void SetEnabled(bool enabled);

        // Returns is enabled. Not enabled listeners doesn't react on shortcut
        bool IsEnabled() const; 

    private:
        ShortcutKeys mShortcut;
        bool         mEnabled;

    protected:
        // This event calling when shortcut hit and this listener has max priority
        virtual void OnShortcutPressed() {}

        friend class ShortcutKeysListenersManager;
    };

    class ShortcutKeysListenersManager: public KeyboardEventsListener, public Singleton<ShortcutKeysListenersManager>
    {
    protected:
        Map<ShortcutKeys, Vector<ShortcutKeysListener*>> mListeners;

    protected:
        // Registers listener 
        void Register(const ShortcutKeys& shortcut, ShortcutKeysListener* listener);

        // Unregisters listener
        void UnRegister(const ShortcutKeys& shortcut, ShortcutKeysListener* listener);

        // Set listener minimal priority
        void SetMinPriority(const ShortcutKeys& shortcut, ShortcutKeysListener* listener);

        // Set listener maximal priority
        void SetMaxPriority(const ShortcutKeys& shortcut, ShortcutKeysListener* listener);

        // Called when key was pressed, send event to most priority listener
        void OnKeyPressed(const Input::Key& key) override;

        friend class ShortcutKeysListener;
    };
}
