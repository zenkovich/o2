#pragma once

#include "o2/Utils/Types/Ref.h"
#include "o2/Scene/SceneLayer.h"

#include "o2/Scene/Scene.h"

namespace o2
{
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
