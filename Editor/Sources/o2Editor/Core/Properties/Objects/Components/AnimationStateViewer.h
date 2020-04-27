#pragma once

#include "o2/Animation/AnimationState.h"
#include "o2Editor/Core/Properties/Objects/DefaultObjectPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class Toggle;
	class Button;
	class HorizontalProgress;
}

namespace Editor
{
	// ------------------------------------
	// AnimationComponent properties viewer
	// ------------------------------------
	class AnimationStateViewer : public DefaultObjectPropertiesViewer
	{
	public:
		// Default constructor. Initializes fields controls
		AnimationStateViewer();

		// Refreshing controls and properties by target objects
		void Refresh(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// It is called when viewer preparing to initialize properties
		void Prepare() override;

		// Returns viewing objects type
		const Type* GetViewingObjectType() const override;

		// Returns viewing objects base type by static function
		static const Type* GetViewingObjectTypeStatic();

		IOBJECT(AnimationStateViewer);

	private:
		Toggle* mPlayPause = nullptr;
		Toggle* mLooped = nullptr;

		HorizontalProgress* mTimeProgress = nullptr;

	private:
		// It is called when viewer freeing
		void OnFree() override;

		// It is called when play/pause button toggled
		void OnPlayPauseToggled(bool value);

		// It is called when loop button toggled
		void OnLoopToggled(bool value);

		// It is called when animation has updated
		void OnAnimationUpdate(float time);
	};
}

CLASS_BASES_META(Editor::AnimationStateViewer)
{
	BASE_CLASS(Editor::DefaultObjectPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationStateViewer)
{
	PRIVATE_FIELD(mPlayPause);
	PRIVATE_FIELD(mLooped);
	PRIVATE_FIELD(mTimeProgress);
}
END_META;
CLASS_METHODS_META(Editor::AnimationStateViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

	PUBLIC_FUNCTION(void, Refresh, _tmp1);
	PUBLIC_FUNCTION(void, Prepare);
	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetViewingObjectTypeStatic);
	PRIVATE_FUNCTION(void, OnFree);
}
END_META;
