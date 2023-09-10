#include "o2/stdafx.h"
#include "SceneLayerRef.h"

#include "o2/Scene/Scene.h"

namespace o2
{
	Ref<SceneLayer>::Ref():
		mLayer(Scene::IsSingletonInitialzed() ? o2Scene.GetDefaultLayer() : nullptr)
	{}
	
	Ref<SceneLayer>::Ref(const String& name):
		mLayerName(name), mLayer(Scene::IsSingletonInitialzed() ? o2Scene.GetLayer(name) : nullptr)
	{}
	
	Ref<SceneLayer>::Ref(const Ref<SceneLayer>& other):
		mLayerName(other.mLayerName), mLayer(other.mLayer)
	{}
	
	Ref<SceneLayer>& Ref<SceneLayer>::operator=(const Ref<SceneLayer>& other)
	{
		mLayerName = other.mLayerName;
		mLayer = other.mLayer;

		return *this;
	}

	void Ref<SceneLayer>::OnDeserialized(const DataValue& node)
	{
		mLayer = o2Scene.GetLayer(mLayerName);
	}
	
	SceneLayer& Ref<SceneLayer>::Get()
	{
		return *mLayer;
	}
	
	const SceneLayer& Ref<SceneLayer>::Get() const
	{
		return *mLayer;
	}
	
	bool Ref<SceneLayer>::operator!=(const Ref<SceneLayer>& other) const
	{
		return mLayer != other.mLayer;
	}
	
	bool Ref<SceneLayer>::operator==(const Ref<SceneLayer>& other) const
	{
		return mLayer == other.mLayer;
	}
	
	const SceneLayer* Ref<SceneLayer>::operator->() const
	{
		return mLayer;
	}
	
	SceneLayer* Ref<SceneLayer>::operator->()
	{
		return mLayer;
	}
	
	const SceneLayer& Ref<SceneLayer>::operator*() const
	{
		return *mLayer;
	}
	
	SceneLayer& Ref<SceneLayer>::operator*()
	{
		return *mLayer;
	}
}

DECLARE_CLASS_MANUAL(o2::Ref<o2::SceneLayer>);
