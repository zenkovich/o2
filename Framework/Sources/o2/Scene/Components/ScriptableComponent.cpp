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

	ScriptValue ScriptableComponent::GetInstance() const
	{
		return mInstance;
	}

	ScriptValue ScriptableComponent::GetClass() const
	{
		return mClass;
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
		return "ui/UI4_component_icon.png";
	}

	void ScriptableComponent::LoadScriptAndCreateObject()
	{
		if (!mScript)
			return;

		auto parseRes = mScript->Parse();
		o2Scripts.Run(parseRes);

		auto className = o2FileSystem.GetPathWithoutDirectories(o2FileSystem.GetFileNameWithoutExtension(mScript->GetPath()));
		auto classObj = o2Scripts.GetGlobal().GetOwnProperty(ScriptValue(className));
		if (classObj.IsConstructor())
		{
			mInstance = classObj.Construct({});
			mClass = classObj;

			mOnStartFunc = mInstance.GetProperty("OnStart");
			mUpdateEnabledFunc = mInstance.GetProperty("UpdateEnabled");
			mOnEnabledFunc = mInstance.GetProperty("OnEnabled");
			mOnDisabledFunc = mInstance.GetProperty("OnDisabled");
			mUpdateFunc = mInstance.GetProperty("Update");
		}
		else
		{
			o2Debug.LogError("Can't find scripting class '" + className + "' in component file - " + mScript->GetPath());

			mInstance = ScriptValue();
			mClass = ScriptValue();
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

		if (mInstance.IsObject())
			node["mInstance"].Set(mInstance);
	}

	void ScriptableComponent::OnDeserialized(const DataValue& node)
	{
		Component::OnDeserialized(node);

		node["mScript"].Get(mScript);
		LoadScriptAndCreateObject();

		if (mInstance.IsObject())
		{
			if (auto objectNode = node.FindMember("mInstance"))
				objectNode->Get(mInstance);
		}
	}

	void ScriptableComponent::SetOwnerActor(Actor* actor)
	{
		Component::SetOwnerActor(actor);

		//if (mInstance.IsObject())
			mInstance.SetProperty("_actor", actor);
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
			mOnStartFunc.InvokeRaw(mInstance, {});
	}

	void ScriptableComponent::Update(float dt)
	{
		if (mUpdateFunc.IsFunction())
			mUpdateFunc.Invoke<void, float>(mInstance, dt);
	}

	void ScriptableComponent::UpdateEnabled()
	{
		if (mUpdateEnabledFunc.IsFunction())
			mUpdateEnabledFunc.InvokeRaw(mInstance, {});
	}

	void ScriptableComponent::OnEnabled()
	{
		if (mOnEnabledFunc.IsFunction())
			mOnEnabledFunc.InvokeRaw(mInstance, {});
	}

	void ScriptableComponent::OnDisabled()
	{
		if (mOnDisabledFunc.IsFunction())
			mOnDisabledFunc.InvokeRaw(mInstance, {});
	}

	void ScriptableComponent::OnTransformChanged()
	{

	}

	void ScriptableComponent::OnTransformUpdated()
	{

	}
}

DECLARE_CLASS(o2::ScriptableComponent);
