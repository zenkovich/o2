#pragma once

#include "o2/Scene/Component.h"
#include "o2/Assets/Types/JavaScriptAsset.h"

namespace o2
{	
	// ---------------------
	// Java Script component
	// ---------------------
	class ScriptableComponent: public Component
	{
	public:
		PROPERTIES(ScriptableComponent);
		PROPERTY(JavaScriptAssetRef, script, SetScript, GetScript); // Script asset link property

	public:
		// Default constructor
		ScriptableComponent();

		// Copy-constructor
		ScriptableComponent(const ScriptableComponent& other);

		// Destructor
		~ScriptableComponent();

		// Copy-operator
		ScriptableComponent& operator=(const ScriptableComponent& other);

		// Calls update
		void Update(float dt) override;

		// Sets script
		void SetScript(const JavaScriptAssetRef& script);

		// Returns script
		const JavaScriptAssetRef& GetScript() const;

		// Returns scripting instance
		ScriptValue GetInstance() const;

		// Returns scripting instance class
		ScriptValue GetClass() const;

		// Returns name of component
		static String GetName();

		// Returns category of component
		static String GetCategory();

		// Returns name of component icon
		static String GetIcon();

		SERIALIZABLE(ScriptableComponent);

	protected:
		JavaScriptAssetRef mScript; // Java script asset link

		ScriptValue mInstance;  // Script value instance @EDITOR_PROPERTY @NO_HEADER
		ScriptValue mClass;     // Script instance class

		ScriptValue mOnStartFunc;
		ScriptValue mUpdateEnabledFunc;
		ScriptValue mOnEnabledFunc;
		ScriptValue mOnDisabledFunc;
		ScriptValue mUpdateFunc;

	protected:
		// Loads script and creates object instance
		void LoadScriptAndCreateObject();

		// Beginning serialization callback
		void OnSerialize(DataValue& node) const override;

		// Completion deserialization callback, loads script and creates instance
		void OnDeserialized(const DataValue& node) override;

		// Sets owner actor
		void SetOwnerActor(Actor* actor) override;

		// It is called when actor was included to scene
		void OnAddToScene() override;

		// It is called when actor was excluded from scene
		void OnRemoveFromScene() override;

		// It is called when component started working on first update frame
		void OnStart() override;

		// Updates component enable
		void UpdateEnabled() override;

		// Is is called when actor enabled in hierarchy
		void OnEnabled() override;

		// It is called when actor disabled in hierarchy
		void OnDisabled() override;

		// It is called when transformation was changed 
		void OnTransformChanged() override;

		// It is called when actor's transform was changed
		void OnTransformUpdated() override;
	};
}

CLASS_BASES_META(o2::ScriptableComponent)
{
	BASE_CLASS(o2::Component);
}
END_META;
CLASS_FIELDS_META(o2::ScriptableComponent)
{
	FIELD().PUBLIC().NAME(script);
	FIELD().PROTECTED().NAME(mScript);
	FIELD().PROTECTED().EDITOR_PROPERTY_ATTRIBUTE().NO_HEADER_ATTRIBUTE().NAME(mInstance);
	FIELD().PROTECTED().NAME(mClass);
	FIELD().PROTECTED().NAME(mOnStartFunc);
	FIELD().PROTECTED().NAME(mUpdateEnabledFunc);
	FIELD().PROTECTED().NAME(mOnEnabledFunc);
	FIELD().PROTECTED().NAME(mOnDisabledFunc);
	FIELD().PROTECTED().NAME(mUpdateFunc);
}
END_META;
CLASS_METHODS_META(o2::ScriptableComponent)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const ScriptableComponent&);
	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SIGNATURE(void, SetScript, const JavaScriptAssetRef&);
	FUNCTION().PUBLIC().SIGNATURE(const JavaScriptAssetRef&, GetScript);
	FUNCTION().PUBLIC().SIGNATURE(ScriptValue, GetInstance);
	FUNCTION().PUBLIC().SIGNATURE(ScriptValue, GetClass);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetName);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCategory);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetIcon);
	FUNCTION().PROTECTED().SIGNATURE(void, LoadScriptAndCreateObject);
	FUNCTION().PROTECTED().SIGNATURE(void, OnSerialize, DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDeserialized, const DataValue&);
	FUNCTION().PROTECTED().SIGNATURE(void, SetOwnerActor, Actor*);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAddToScene);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRemoveFromScene);
	FUNCTION().PROTECTED().SIGNATURE(void, OnStart);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnDisabled);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTransformChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, OnTransformUpdated);
}
END_META;
