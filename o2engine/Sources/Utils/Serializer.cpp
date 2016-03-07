#include "Serializer.h"

#include "Scene/Actor.h"

namespace o2
{
	void Serializer::Serialize(Actor* actor, DataNode& data)
	{
		if (actor->IsAsset())
		{
			*data.AddNode("AssetId") = actor->GetAssetId();
		}
		else if (actor->IsOnScene())
		{
			*data.AddNode("SceneId") = actor->GetId();
		}
		else
		{
			*data.AddNode("Data") = actor ? actor->Serialize() : (String)"null";
		}
	}

	void Serializer::Deserialize(Actor*& actor, DataNode& data)
	{
		if (auto assetIdNode = data.GetNode("AssetId"))
		{
			AssetId assetId = *assetIdNode;
			actor = o2Scene.GetAssetActorByID(assetId);
		}
		else if (auto sceneIdNode = data.GetNode("SceneId"))
		{
			actor = o2Scene.GetActorByID(*sceneIdNode);
		}
		else if (auto dataNode = data.GetNode("Data"))
		{
			if (dataNode->Data() == "null")
				actor = nullptr;
			else
			{
				actor = mnew Actor();
				actor->ExcludeFromScene();
				actor->Deserialize(*dataNode);
			}
		}
	}
}