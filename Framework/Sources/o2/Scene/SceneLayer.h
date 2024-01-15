#pragma once

#include "o2/Scene/ISceneDrawable.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/Ref.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
    class Actor;
    class ISceneDrawable;

    // --------------------------------------------------------------------------------
    // Scene layer. It contains Actors and their Drawable parts, managing sorting order
    // --------------------------------------------------------------------------------
    class SceneLayer: public ISerializable, public RefCounterable
    {
    public:
#if IS_EDITOR
        bool visible = true; // Is layer visible in editor
#endif

    public:
        // ------------------------------------------------------------------------------------
        // Root drawables container. It is used to draw all root drawables with inherited depth
        // ------------------------------------------------------------------------------------
        struct RootDrawablesContainer : public ISceneDrawable
        {};

    public:
        // Default constructor
        SceneLayer();

        // Sets layer name
        void SetName(const String& name);

        // Returns layer name
        const String& GetName() const;

        // Returns all drawable objects of actors in layer
        const Vector<WeakRef<ISceneDrawable>>& GetDrawables() const;

        // Returns root drawable objects of actors in layer
        RootDrawablesContainer& GetRootDrawables();

        SERIALIZABLE(SceneLayer);

    protected:
        String mName; // Name of layer @SERIALIZABLE

        Vector<WeakRef<ISceneDrawable>> mDrawables; // Drawable objects in layer

        RootDrawablesContainer mRootDrawables; // Root drawables with inherited depth. Draws at 0 priority

    protected:
        // Registers drawable object
        void RegisterDrawable(ISceneDrawable* drawable);

        // Unregisters drawable object
        void UnregisterDrawable(ISceneDrawable* drawable);

        // Sets drawable order as last of all objects with same depth
        void SetLastByDepth(const Ref<ISceneDrawable>& drawable);

        friend class Actor;
        friend class CameraActor;
        friend class DrawableComponent;
        friend class Scene;
        friend class ISceneDrawable;
        friend class Widget;
    };
}
// --- META ---

CLASS_BASES_META(o2::SceneLayer)
{
    BASE_CLASS(o2::ISerializable);
    BASE_CLASS(o2::RefCounterable);
}
END_META;
CLASS_FIELDS_META(o2::SceneLayer)
{
#if  IS_EDITOR
    FIELD().PUBLIC().DEFAULT_VALUE(true).NAME(visible);
#endif
    FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().NAME(mName);
    FIELD().PROTECTED().NAME(mDrawables);
    FIELD().PROTECTED().NAME(mRootDrawables);
}
END_META;
CLASS_METHODS_META(o2::SceneLayer)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().SIGNATURE(void, SetName, const String&);
    FUNCTION().PUBLIC().SIGNATURE(const String&, GetName);
    FUNCTION().PUBLIC().SIGNATURE(const Vector<WeakRef<ISceneDrawable>>&, GetDrawables);
    FUNCTION().PUBLIC().SIGNATURE(RootDrawablesContainer&, GetRootDrawables);
    FUNCTION().PROTECTED().SIGNATURE(void, RegisterDrawable, ISceneDrawable*);
    FUNCTION().PROTECTED().SIGNATURE(void, UnregisterDrawable, ISceneDrawable*);
    FUNCTION().PROTECTED().SIGNATURE(void, SetLastByDepth, const Ref<ISceneDrawable>&);
}
END_META;
// --- END META ---
