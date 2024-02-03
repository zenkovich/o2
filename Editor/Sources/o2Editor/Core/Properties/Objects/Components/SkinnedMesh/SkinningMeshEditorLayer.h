#include "o2/Render/Sprite.h"
#include "o2/Scene/Components/SkinningMeshComponent.h"
#include "o2Editor/SceneWindow/SceneEditorLayer.h"
#include "o2/Core/Ref.h"
#include "o2/Core/WeakRef.h"

using namespace o2;

namespace Editor
{
	// -----------------------------------------------------------------
	// Scene edit layer, used to draw mesh's texture frame and mesh wire
	// -----------------------------------------------------------------
	struct SkinningMeshEditorLayer : public SceneEditorLayer
	{
		bool drawMeshWire = true;

		Ref<SkinningMeshComponent> mesh;
		Sprite textureSprite;

	public:
		void DrawOverScene() override;
		void Update(float dt) override;

		int GetOrder() const override;

		bool IsEnabled() const override;

		const String& GetName() const override;
		const String& GetIconName() const override;

		void DrawMeshWire();
	};
}