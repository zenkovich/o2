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
        const Ref<RootDrawablesContainer>& GetRootDrawables();

        SERIALIZABLE(SceneLayer);

    protected:
        String mName; // Name of layer @SERIALIZABLE

        Vector<WeakRef<ISceneDrawable>> mDrawables; // Drawable objects in layer

        Ref<RootDrawablesContainer> mRootDrawables; // Root drawables with inherited depth. Draws at 0 priority

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

    // ---------------------------------------------------------------------
    // Reference to scene layer. Stores by name, creates new layer if needed
    // ---------------------------------------------------------------------
    template<typename T>
    class Ref<T, typename std::enable_if<std::is_same<SceneLayer, T>::value>::type> : public ISerializable, public BaseRef<SceneLayer>
    {
    public:
        using Base = BaseRef<SceneLayer>;

    public:
        // Base reference implementation
        BASE_REF_IMPLEMETATION(SceneLayer);

        // Constructor from layer name
        explicit Ref(const String& name) :
            mLayerName(name)
        {
            *this = Scene::IsSingletonInitialzed() ? o2Scene.GetLayer(name) : nullptr;
        }

    protected:
        String mLayerName = String("Default"); // Name of layer @SERIALIZABLE

    protected:
        // Gets layer by name
        void OnDeserialized(const DataValue& node) override
        {
            *this = o2Scene.GetLayer(mLayerName).Get();
        }

    public:
        typedef Ref<SceneLayer> _thisType;

        SERIALIZABLE_MAIN(_thisType);
        IOBJECT_SCRIPTING();

        template<typename _type_processor>
        static void ProcessBaseTypes(_thisType* object, _type_processor& processor)
        {
            typedef _thisType thisclass;
            processor.template StartBases<_thisType>(object, type);

            BASE_CLASS(o2::ISerializable);
        }

        template<typename _type_processor>
        static void ProcessFields(_thisType* object, _type_processor& processor)
        {
            typedef _thisType thisclass;
            processor.template StartFields<_thisType>(object, type);

            //FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(String("Default")).NAME(mLayerName);
            FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(Base::mPtr);
        }

        template<typename _type_processor>
        static void ProcessMethods(_thisType* object, _type_processor& processor)
        {
            typedef _thisType thisclass;
            processor.template StartMethods<_thisType>(object, type);

            FUNCTION().PUBLIC().CONSTRUCTOR();
            FUNCTION().PUBLIC().CONSTRUCTOR(const String&);
            FUNCTION().PUBLIC().CONSTRUCTOR(const Ref&);
            FUNCTION().PUBLIC().SIGNATURE(SceneLayer&, Get);
            FUNCTION().PUBLIC().SIGNATURE(const SceneLayer&, Get);
        }
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
    FUNCTION().PUBLIC().SIGNATURE(const Ref<RootDrawablesContainer>&, GetRootDrawables);
    FUNCTION().PROTECTED().SIGNATURE(void, RegisterDrawable, ISceneDrawable*);
    FUNCTION().PROTECTED().SIGNATURE(void, UnregisterDrawable, ISceneDrawable*);
    FUNCTION().PROTECTED().SIGNATURE(void, SetLastByDepth, const Ref<ISceneDrawable>&);
}
END_META;
// --- END META ---
