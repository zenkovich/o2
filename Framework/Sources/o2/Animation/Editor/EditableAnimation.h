#pragma once

namespace o2
{
    // ---------------------------------------------------
    // Animation, that can be edited from animation editor
    // ---------------------------------------------------
    class IEditableAnimation
    {
    public:
        // Called when animation started to edit. It means that animation must be deactivated
        virtual void BeginAnimationEdit() = 0;

        // Called when animation finished editing. ANimation must be reactivated
        virtual void EndAnimationEdit() = 0;
    };
}
