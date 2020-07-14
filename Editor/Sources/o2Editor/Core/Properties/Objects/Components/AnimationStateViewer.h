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
		// Returns viewing objects type
		const Type* GetViewingObjectType() const override;

		// Returns viewing objects base type by static function
		static const Type* GetViewingObjectTypeStatic();

		IOBJECT(AnimationStateViewer);

	private:
		Toggle* mPlayPause = nullptr;
		Button* mEditBtn = nullptr;
		Toggle* mLooped = nullptr;

		HorizontalProgress* mTimeProgress = nullptr;

		AnimationPlayer* mSubscribedPlayer = nullptr;

	private:
		// Creates spoiler for properties
		Spoiler* CreateSpoiler() override;

		// It is called when viewer is refreshed
		void OnRefreshed(const Vector<Pair<IObject*, IObject*>>& targetObjets) override;

		// This is called when the viewer is freed
		void OnFree() override;

		// It is called when play pause toggled
		void OnPlayPauseToggled(bool play);

		// It is called when loop toggled
		void OnLoopToggled(bool looped);

		// It is called when edit button pressed, sets animation editing
		void OnEditPressed();

		// It is called when time progress changed by user, sets subscribed player time 
		void OnTimeProgressChanged(float value);

		// It is called when animation updates
		void OnAnimationUpdated(float time);
	};
}

CLASS_BASES_META(Editor::AnimationStateViewer)
{
	BASE_CLASS(Editor::DefaultObjectPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationStateViewer)
{
	PRIVATE_FIELD(mPlayPause).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mEditBtn).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mLooped).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mTimeProgress).DEFAULT_VALUE(nullptr);
	PRIVATE_FIELD(mSubscribedPlayer).DEFAULT_VALUE(nullptr);
}
END_META;
CLASS_METHODS_META(Editor::AnimationStateViewer)
{

	typedef const Vector<Pair<IObject*, IObject*>>& _tmp1;

	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_STATIC_FUNCTION(const Type*, GetViewingObjectTypeStatic);
	PRIVATE_FUNCTION(Spoiler*, CreateSpoiler);
	PRIVATE_FUNCTION(void, OnRefreshed, _tmp1);
	PRIVATE_FUNCTION(void, OnFree);
	PRIVATE_FUNCTION(void, OnPlayPauseToggled, bool);
	PRIVATE_FUNCTION(void, OnLoopToggled, bool);
	PRIVATE_FUNCTION(void, OnEditPressed);
	PRIVATE_FUNCTION(void, OnTimeProgressChanged, float);
	PRIVATE_FUNCTION(void, OnAnimationUpdated, float);
}
END_META;
