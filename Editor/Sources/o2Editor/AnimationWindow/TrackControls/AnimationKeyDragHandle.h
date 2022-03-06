#pragma once

#include "o2/Utils/Editor/DragHandle.h"

using namespace o2;

namespace Editor
{
	class ITrackControl;

	class AnimationKeyDragHandle : public WidgetDragHandle
	{ 
	public:
		String           trackPath;
		IAnimationTrack* track = nullptr;
		ITrackControl*   trackControl = nullptr;
		UInt64           keyUid = 0;
		bool             isMapping = false;

	public:
		// Default constructor
		AnimationKeyDragHandle();

		// Constructor with views
		AnimationKeyDragHandle(Sprite* regular, Sprite* hover = nullptr, Sprite* pressed = nullptr,
						       Sprite* selected = nullptr, Sprite* selectedHovered = nullptr,
						       Sprite* selectedPressed = nullptr);

		// Copy-constructor
		AnimationKeyDragHandle(const AnimationKeyDragHandle& other);

		// Destructor
		~AnimationKeyDragHandle();

		// Copy-operator
		AnimationKeyDragHandle& operator=(const AnimationKeyDragHandle& other);

		// Draws handle
		void Draw() override;

		// Returns create menu category in editor
		static String GetCreateMenuCategory();

		SERIALIZABLE(AnimationKeyDragHandle);
	};
}

CLASS_BASES_META(Editor::AnimationKeyDragHandle)
{
	BASE_CLASS(o2::WidgetDragHandle);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationKeyDragHandle)
{
	FIELD().PUBLIC().NAME(trackPath);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(track);
	FIELD().PUBLIC().DEFAULT_VALUE(nullptr).NAME(trackControl);
	FIELD().PUBLIC().DEFAULT_VALUE(0).NAME(keyUid);
	FIELD().PUBLIC().DEFAULT_VALUE(false).NAME(isMapping);
}
END_META;
CLASS_METHODS_META(Editor::AnimationKeyDragHandle)
{

	FUNCTION().PUBLIC().SIGNATURE(void, Draw);
	FUNCTION().PUBLIC().SIGNATURE_STATIC(String, GetCreateMenuCategory);
}
END_META;
