#pragma once

#include "Scene/UI/Widgets/Window.h"

using namespace o2;

namespace o2
{
	class EditBox;
}

namespace Editor
{
	class KeyEditWindow : public Window
	{
	public:
		KeyEditWindow();
		KeyEditWindow(const KeyEditWindow& other);
		KeyEditWindow& operator=(const KeyEditWindow& other);

		void Show(const Curve::Key& key, const Function<void(const Curve::Key& key)>& onClosed);

		SERIALIZABLE(KeyEditWindow);

	private:
		EditBox* mPosition;
		EditBox* mValue;
		EditBox* mLeftSupportPosition;
		EditBox* mLeftSupportValue;
		EditBox* mRightSupportPosition;
		EditBox* mRightSupportValue;

		Function<void(const Curve::Key& key)> mOnClosed;

	private:
		// Initializes layout and controls
		void InitializeControls();

		// Searches controls in layout
		void FindControls();

		// It is called when Ok button pressed, calls onClosed function with new key parameters
		void OnOkPressed();
	};
}

CLASS_BASES_META(Editor::KeyEditWindow)
{
	BASE_CLASS(o2::Window);
}
END_META;
CLASS_FIELDS_META(Editor::KeyEditWindow)
{
}
END_META;
CLASS_METHODS_META(Editor::KeyEditWindow)
{

	PUBLIC_FUNCTION(void, Show, const Curve::Key&, const Function<void(const Curve::Key& key)>&);
}
END_META;
