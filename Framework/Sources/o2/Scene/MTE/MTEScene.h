#pragma once

#include "o2/Scene/Actor.h"
#include "o2/Scene/ISceneDrawable.h"

#include "gengine/renderer/renderer_fwd.h"
#include "gengine/renderer/renderer.h"
#include "gengine/graphics/definition.h"
#include "gengine/scene/scene.h"

namespace o2
{
	class MTESceneActor : public Actor, public ISceneDrawable
	{
	public:
		using SceneFactory = std::function<std::shared_ptr<mte::Scene>(
			mte::EventManager* manager, mte::ResourceSubsystem subsystems, std::string_view options)>;

	public:
		// Default constructor
		MTESceneActor();

		// Copy-constructor
		MTESceneActor(const MTESceneActor& other);

		// Copy-operator
		MTESceneActor& operator=(const MTESceneActor& other);

		// Destructor
		~MTESceneActor();

		// Initializes actor with scene factory
		void Initialize(SceneFactory sceneFactory, std::string_view options);

		// Draws content of scene
		void Draw() override;

		SERIALIZABLE(MTESceneActor);

	protected:
		int m_windowWidth;
		int m_windowHeight;

		mte::RendererPtr m_render;
		mte::VideoFactoryPtr m_video;
		mte::FileServerPtr m_fileserver;
		mte::EventManagerPtr m_events;

		std::shared_ptr<mte::Scene> m_scene;

	protected:
		// Called when result enable was changed
		void OnEnableInHierarchyChanged() override;

		// Called when actor has added to scene; creates rigid body
		void OnAddToScene() override;

		// Called when actor has removed from scene; destroys rigid body
		void OnRemoveFromScene() override;

		// Called on update with frame dt
		void OnUpdate(float dt) override;

		// Returns current scene layer
		SceneLayer* GetSceneDrawableSceneLayer() const override;

		// Returns is drawable enabled
		bool IsSceneDrawableEnabled() const override;

		// Returns parent scene drawable
		ISceneDrawable* GetParentDrawable() override;

		// Returns the index in the parent's list of children, used to sort the rendering
		int GetIndexInParentDrawable() const override;
	};
}

CLASS_BASES_META(o2::MTESceneActor)
{
	BASE_CLASS(o2::Actor);
	BASE_CLASS(o2::ISceneDrawable);
}
END_META;
CLASS_FIELDS_META(o2::MTESceneActor)
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
CLASS_METHODS_META(o2::MTESceneActor)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(const MTESceneActor&);
	FUNCTION().PUBLIC().SIGNATURE(void, Initialize, SceneFactory, std::string_view);
	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PROTECTED().SIGNATURE(void, OnEnableInHierarchyChanged);
	FUNCTION().PROTECTED().SIGNATURE(void, OnAddToScene);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRemoveFromScene);
	FUNCTION().PROTECTED().SIGNATURE(void, OnUpdate, float);
	FUNCTION().PROTECTED().SIGNATURE(SceneLayer*, GetSceneDrawableSceneLayer);
	FUNCTION().PROTECTED().SIGNATURE(bool, IsSceneDrawableEnabled);
	FUNCTION().PROTECTED().SIGNATURE(ISceneDrawable*, GetParentDrawable);
	FUNCTION().PROTECTED().SIGNATURE(int, GetIndexInParentDrawable);
}
END_META;
