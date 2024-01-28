#include "o2/stdafx.h"
#include "SceneLayerRef.h"

#include "o2/Scene/Scene.h"

namespace o2
{
    Ref<SceneLayer>::Ref():
        mLayer(Scene::IsSingletonInitialzed() ? const_cast<SceneLayer*>(o2Scene.GetDefaultLayer().Get()) : nullptr)
    {}
    
    Ref<SceneLayer>::Ref(const String& name):
        mLayerName(name), mLayer(Scene::IsSingletonInitialzed() ? o2Scene.GetLayer(name).Get() : nullptr)
    {}

    Ref<SceneLayer>::Ref(SceneLayer* layer):
        mLayerName(layer ? layer->GetName() : ""), mLayer(layer)
    {}
    
    Ref<SceneLayer>::Ref(const Ref<SceneLayer>& other):
        mLayerName(other.mLayerName), mLayer(Scene::IsSingletonInitialzed() ? o2Scene.GetLayer(other.mLayerName).Get() : nullptr)
    {}
    
    Ref<SceneLayer>& Ref<SceneLayer>::operator=(const Ref<SceneLayer>& other)
    {
        mLayerName = other.mLayerName;
        mLayer = o2Scene.GetLayer(mLayerName).Get();

        return *this;
    }

    void Ref<SceneLayer>::OnDeserialized(const DataValue& node)
    {
        mLayer = o2Scene.GetLayer(mLayerName).Get();
    }
    
    SceneLayer* Ref<SceneLayer>::Get()
    {
        return mLayer;
    }
    
    const SceneLayer* Ref<SceneLayer>::Get() const
    {
        return mLayer;
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

DECLARE_CLASS_MANUAL(o2::Ref<o2::SceneLayer>, o2_Ref_SceneLayer);
