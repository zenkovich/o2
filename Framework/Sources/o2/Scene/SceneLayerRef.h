#pragma once

#include "o2/Scene/SceneLayer.h"
#include "o2/Utils/Types/Ref.h"

namespace o2
{
	// ---------------------------------------------------------------------
	// Reference to scene layer. Stores by name, creates new layer if needed
	// ---------------------------------------------------------------------
	template<>
	class Ref<SceneLayer> : public ISerializable
	{
	public:
		// Default constructor, references to default
		Ref();

		// Constructor from layer name
		explicit Ref(const String& name);

		// Copy constructor
		Ref(const Ref& other);

		// Assign operator
		Ref& operator=(const Ref& other);

		// Returns layer reference
		SceneLayer& Get();

		// Returns constant layer reference
		const SceneLayer& Get() const;

		// Returns layer reference
		SceneLayer& operator*();

		// Returns constant layer reference
		const SceneLayer& operator*() const;

		// Returns layer reference
		SceneLayer* operator->();

		// Returns constant layer reference
		const SceneLayer* operator->() const;

		// Check equals operator
		bool operator==(const Ref<SceneLayer>& other) const;

		// Check not equals operator
		bool operator!=(const Ref<SceneLayer>& other) const;

	protected:
		String mLayerName = String("Default"); // Name of layer @SERIALIZABLE

		SceneLayer* mLayer = nullptr; // Layer reference

	protected:
		// Gets layer by name
		void OnDeserialized(const DataValue& node) override;

	public:
		typedef Ref<SceneLayer> _thisType;

		SERIALIZABLE_MAIN(_thisType);

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

			FIELD().PROTECTED().SERIALIZABLE_ATTRIBUTE().DEFAULT_VALUE(String("Default")).NAME(mLayerName);
			FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLayer);
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
