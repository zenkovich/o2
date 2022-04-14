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

	void ScriptableComponent::OnDeserialized(const DataValue& node)
	{
		LoadScriptAndCreateObject();
	}

	void ScriptableComponent::SetOwnerActor(Actor* actor)
	{
		Component::SetOwnerActor(actor);
	}

	void ScriptableComponent::OnAddToScene()
	{

	}

	void ScriptableComponent::OnRemoveFromScene()
	{

	}

	void ScriptableComponent::OnStart()
	{
		if (mOnStartFunc.GetValueType() == ScriptValue::ValueType::Function)
			mOnStartFunc.InvokeRaw(mObject, {});
	}

	void ScriptableComponent::Update(float dt)
	{
		if (mUpdateFunc.GetValueType() == ScriptValue::ValueType::Function)
			mUpdateFunc.Invoke<void, float>(mObject, dt);
	}

	void ScriptableComponent::UpdateEnabled()
	{
		if (mUpdateEnabledFunc.GetValueType() == ScriptValue::ValueType::Function)
			mUpdateEnabledFunc.InvokeRaw(mObject, {});
	}

	void ScriptableComponent::OnEnabled()
	{
		if (mOnEnabledFunc.GetValueType() == ScriptValue::ValueType::Function)
			mOnEnabledFunc.InvokeRaw(mObject, {});
	}

	void ScriptableComponent::OnDisabled()
	{
		if (mOnDisabledFunc.GetValueType() == ScriptValue::ValueType::Function)
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
