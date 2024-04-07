#pragma once

#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/Ref.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
    class Actor;

    FORWARD_CLASS_REF(SceneLayerRootDrawablesContainer);
    FORWARD_CLASS_REF(ISceneDrawable);

    // --------------------------------------------------------------------------------
    // Scene layer. It contains Actors and their Drawable parts, managing sorting order
    // --------------------------------------------------------------------------------
    class SceneLayer: public ISerializable, public RefCounterable, public ICloneableRef
    {
    public:
#if IS_EDITOR
        bool visible = true; // Is layer visible in editor
#endif

    public:
        // Default constructor
        SceneLayer();

        // Sets layer name
        void SetName(const String& name);

        // Returns layer name
        const String& GetName() const;

        // Returns all drawable objects of actors in layer
        const Vector<Ref<ISceneDrawable>>& GetDrawables() const;

        // Returns root drawable objects of actors in layer
        const Ref<SceneLayerRootDrawablesContainer>& GetRootDrawables();

        SERIALIZABLE(SceneLayer);
        CLONEABLE_REF(SceneLayer);

    protected:
        String mName; // Name of layer @SERIALIZABLE

        Vector<Ref<ISceneDrawable>> mDrawables; // Drawable objects in layer

        Ref<SceneLayerRootDrawablesContainer> mRootDrawables; // Root drawables with inherited depth. Draws at 0 priority

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
        explicit Ref(const String& name);

        void SetName(const String& name);

    protected:
        String mLayerName = String("Default"); // Name of layer @SERIALIZABLE

    protected:
        // Gets layer by name
        void OnDeserialized(const DataValue& node) override;

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

#include "o2/Scene/Scene.h"

namespace o2
{
    template<typename T>
    Ref<T, typename std::enable_if<std::is_same<SceneLayer, T>::value>::type>::Ref(const String& name) :
        mLayerName(name)
    {
        *this = Scene::IsSingletonInitialzed() ? o2Scene.GetLayer(name) : nullptr;
    }

    template<typename T>
    void Ref<T, typename std::enable_if<std::is_same<SceneLayer, T>::value>::type>::SetName(const String& name)
    {
        mLayerName = name;
        *this = o2Scene.GetLayer(name);
    }

    template<typename T>
    void Ref<T, typename std::enable_if<std::is_same<SceneLayer, T>::value>::type>::OnDeserialized(const DataValue& node)
    {
        *this = o2Scene.GetLayer(mLayerName).Get();
    }
}
// --- META ---

CLASS_BASES_META(o2::SceneLayer)
{
    BASE_CLASS(o2::ISerializable);
    BASE_CLASS(o2::RefCounterable);
    BASE_CLASS(o2::ICloneableRef);
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
    FUNCTION().PUBLIC().SIGNATURE(const Vector<Ref<ISceneDrawable>>&, GetDrawables);
    FUNCTION().PUBLIC().SIGNATURE(const Ref<SceneLayerRootDrawablesContainer>&, GetRootDrawables);
    FUNCTION().PROTECTED().SIGNATURE(void, RegisterDrawable, ISceneDrawable*);
    FUNCTION().PROTECTED().SIGNATURE(void, UnregisterDrawable, ISceneDrawable*);
    FUNCTION().PROTECTED().SIGNATURE(void, SetLastByDepth, const Ref<ISceneDrawable>&);
}
END_META;
// --- END META ---
