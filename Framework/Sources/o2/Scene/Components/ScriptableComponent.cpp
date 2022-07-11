#include "o2/stdafx.h"
#include "ScriptableComponent.h"

#include "o2/Utils/FileSystem/FileSystem.h"

namespace o2
{
	ScriptableComponent::ScriptableComponent()
	{

	}

	ScriptableComponent::ScriptableComponent(const ScriptableComponent& other):
		Component(other)
	{
		SetScript(other.mScript);
	}

	ScriptableComponent::~ScriptableComponent()
	{

	}

	ScriptableComponent& ScriptableComponent::operator=(const ScriptableComponent& other)
	{
		Component::operator=(other);
		return *this;
	}

	void ScriptableComponent::SetScript(const JavaScriptAssetRef& script)
	{
		mScript = script;
		LoadScriptAndCreateObject();
	}

	const JavaScriptAssetRef& ScriptableComponent::GetScript() const
	{
		return mScript;
	}

	String ScriptableComponent::GetName()
	{
		return "Scriptable";
	}

	String ScriptableComponent::GetCategory()
	{
		return "Basic";
	}

	String ScriptableComponent::GetIcon()
	{
		return "ui/UI4_animation_component.png";
	}

	void ScriptableComponent::LoadScriptAndCreateObject()
	{
		if (!mScript)
			return;

		auto parseRes = mScript->Parse();
		o2Scripts.Run(parseRes);

		auto className = o2FileSystem.GetFileNameWithoutExtension(mScript->GetPath());
		auto classObj = o2Scripts.GetGlobal().GetOwnProperty(ScriptValue(className));
		if (classObj.GetValueType() == ScriptValue::ValueType::Constructor)
		{
			mObject = classObj.Construct({});

			mOnStartFunc = mObject.GetProperty("OnStart");
			mUpdateEnabledFunc = mObject.GetProperty("UpdateEnabled");
			mOnEnabledFunc = mObject.GetProperty("OnEnabled");
			mOnDisabledFunc = mObject.GetProperty("OnDisabled");
			mUpdateFunc = mObject.GetProperty("Update");
		}
		else
		{
			o2Debug.LogError("Can't find scripting class '" + className + "' in component file - " + mScript->GetPath());

			mObject = ScriptValue();
			mOnStartFunc = ScriptValue();
			mUpdateEnabledFunc = ScriptValue();
			mOnEnabledFunc = ScriptValue();
			mOnDisabledFunc = ScriptValue();
			mUpdateFunc = ScriptValue();
		}
	}

	void ScriptableComponent::OnSerialize(DataValue& node) const
	{
		Component::OnSerialize(node);

		node["mScript"].Set(mScript);

		if (mObject.IsObject())
			node["mObject"].Set(mObject);
	}

	void ScriptableComponent::OnDeserialized(const DataValue& node)
	{
		Component::OnDeserialized(node);

		node["mScript"].Get(mScript);
		LoadScriptAndCreateObject();

		if (mObject.IsObject())
		{
			if (auto objectNode = node.FindMember("mObject"))
				objectNode->Get(mObject);
		}
	}

	void ScriptableComponent::SetOwnerActor(Actor* actor)
	{
		Component::SetOwnerActor(actor);

		if (mObject.IsObject())
			mObject.SetProperty("_actor", actor);
	}

	void ScriptableComponent::OnAddToScene()
	{
		Component::OnAddToScene();
	}

	void ScriptableComponent::OnRemoveFromScene()
	{
		Component::OnRemoveFromScene();
	}

	void ScriptableComponent::OnStart()
	{
		if (mOnStartFunc.IsFunction())
			mOnStartFunc.InvokeRaw(mObject, {});
	}

	void ScriptableComponent::Update(float dt)
	{
		if (mUpdateFunc.IsFunction())
			mUpdateFunc.Invoke<void, float>(mObject, dt);
	}

	void ScriptableComponent::UpdateEnabled()
	{
		if (mUpdateEnabledFunc.IsFunction())
			mUpdateEnabledFunc.InvokeRaw(mObject, {});
	}

	void ScriptableComponent::OnEnabled()
	{
		if (mOnEnabledFunc.IsFunction())
			mOnEnabledFunc.InvokeRaw(mObject, {});
	}

	void ScriptableComponent::OnDisabled()
	{
		if (mOnDisabledFunc.IsFunction())
			mOnDisabledFunc.InvokeRaw(mObject, {});
	}

	void ScriptableComponent::OnTransformChanged()
	{

	}

	void ScriptableComponent::OnTransformUpdated()
	{

	}
}

DECLARE_CLASS(o2::ScriptableComponent);
