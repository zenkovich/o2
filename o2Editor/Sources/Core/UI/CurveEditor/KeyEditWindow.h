#pragma once

#include "Scene/UI/Widgets/Window.h"

using namespace o2;

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
