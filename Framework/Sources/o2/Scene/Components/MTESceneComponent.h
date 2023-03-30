#pragma once

#include "gengine/renderer/renderer_fwd.h"
#include "gengine/renderer/renderer.h"
#include "gengine/graphics/definition.h"
#include "gengine/scene/scene.h"
#include "o2/Scene/DrawableComponent.h"

namespace o2
{
	class MTESceneComponent : public DrawableComponent
	{
	public:
		using SceneFactory = std::function<std::shared_ptr<mte::Scene>(
			mte::EventManager* manager, mte::ResourceSubsystem subsystems, std::string_view options)>;

	public:
		// Default constructor
		MTESceneComponent();

		// Copy-constructor
		MTESceneComponent(const MTESceneComponent& other);

		// Copy-operator
		MTESceneComponent& operator=(const MTESceneComponent& other);

		// Destructor
		~MTESceneComponent();

		// Initializes actor with scene factory
		void Initialize(SceneFactory sceneFactory, std::string_view options);

		// Updates component and mte scene
		void Update(float dt) override;

		// Draws content of scene
		void Draw() override;

		SERIALIZABLE(MTESceneComponent);

	protected:
		int m_windowWidth;
		int m_windowHeight;

		mte::RendererPtr m_render;
		mte::VideoFactoryPtr m_video;
		mte::FileServerPtr m_fileserver;
		mte::EventManagerPtr m_events;

		std::shared_ptr<mte::Scene> m_scene;
	};
}

CLASS_BASES_META(o2::MTESceneComponent)
{
	BASE_CLASS(o2::DrawableComponent);
}
END_META;
CLASS_FIELDS_META(o2::MTESceneComponent)
{
	FIELD().PROTECTED().NAME(m_windowWidth);
	FIELD().PROTECTED().NAME(m_windowHeight);
	FIELD().PROTECTED().NAME(m_render);
	FIELD().PROTECTED().NAME(m_video);
	FIELD().PROTECTED().NAME(m_fileserver);
	FIELD().PROTECTED().NAME(m_events);
	FIELD().PROTECTED().NAME(m_scene);
}
END_META;
CLASS_METHODS_META(o2::MTESceneComponent)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const MTESceneComponent&);
	FUNCTION().PUBLIC().SIGNATURE(void, Initialize, SceneFactory, std::string_view);
	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
}
END_META;
