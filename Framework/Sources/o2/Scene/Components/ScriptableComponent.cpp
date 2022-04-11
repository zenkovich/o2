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

		auto className = o2FileSystem.GetFileNameWithoutExtension(mScript->GetPath());

		auto oldRealm = o2Scripts.SetCurrentRealm(o2Scripts.CreateRealm());

		auto parseRes = mScript->Parse();
		auto classValue = o2Scripts.Eval(className);

		o2Scripts.SetCurrentRealm(oldRealm);

		o2Scripts.Run(parseRes);
		auto t = classValue.GetValueType();

		mObject = classValue.Construct({});
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
		if (mObject.GetValueType() == ScriptValue::ValueType::Object)
			mObject.GetProperty("OnStart").InvokeRaw(mObject, {});
	}

	void ScriptableComponent::Update(float dt)
	{
		if (mObject.GetValueType() == ScriptValue::ValueType::Object)
			mObject.GetProperty("Update").Invoke<void, float>(mObject, dt);
	}

	void ScriptableComponent::UpdateEnabled()
	{

	}

	void ScriptableComponent::OnEnabled()
	{

	}

	void ScriptableComponent::OnDisabled()
	{

	}

	void ScriptableComponent::OnTransformChanged()
	{

	}

	void ScriptableComponent::OnTransformUpdated()
	{

	}
}

DECLARE_CLASS(o2::ScriptableComponent);
