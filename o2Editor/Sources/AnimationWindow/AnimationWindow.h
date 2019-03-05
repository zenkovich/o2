#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"

using namespace o2;

namespace o2
{
	class Label;
	class LongList;
	class Text;
}

namespace Editor
{
	class AnimationWindow: public IEditorWindow
	{
		IOBJECT(AnimationWindow);

	public:
		// Default constructor
		AnimationWindow();

		// Destructor
		~AnimationWindow();

		// Updates window logic
		void Update(float dt) override;

	protected:
		// Initializes window
		void InitializeWindow();
	};
}

CLASS_BASES_META(Editor::AnimationWindow)
{
	BASE_CLASS(Editor::IEditorWindow);
}
END_META;
CLASS_FIELDS_META(Editor::AnimationWindow)
{
}
END_META;
CLASS_METHODS_META(Editor::AnimationWindow)
{

	PUBLIC_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, InitializeWindow);
}
END_META;
