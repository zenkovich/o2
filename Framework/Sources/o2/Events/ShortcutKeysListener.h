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

    class ShortcutKeysListenersManager: public RefCounterable, public KeyboardEventsListener, public Singleton<ShortcutKeysListenersManager>
    {
    protected:
        Map<ShortcutKeys, Vector<WeakRef<ShortcutKeysListener>>> mListeners;

    protected:
        // Registers listener 
        static void Register(const ShortcutKeys& shortcut, const Ref<ShortcutKeysListener>& listener);

        // Unregisters listener
        static void UnRegister(const ShortcutKeys& shortcut, ShortcutKeysListener* listener);

        // Set listener minimal priority
        static void SetMinPriority(const ShortcutKeys& shortcut, const Ref<ShortcutKeysListener>& listener);

        // Set listener maximal priority
        static void SetMaxPriority(const ShortcutKeys& shortcut, const Ref<ShortcutKeysListener>& listener);

        // Called when key was pressed, send event to most priority listener
        void OnKeyPressed(const Input::Key& key) override;

        REF_COUNTERABLE_IMPL(RefCounterable);

        friend class ShortcutKeysListener;
    };
}
