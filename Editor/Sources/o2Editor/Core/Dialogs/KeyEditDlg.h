#pragma once

#include "o2/Utils/Singleton.h"
#include "o2/Utils/Ref.h"

using namespace o2;

namespace o2
{
	class EditBox;
	class Window;
}

namespace Editor
{
	class KeyEditDlg : public Singleton<KeyEditDlg>
	{
	public:
		KeyEditDlg();
		~KeyEditDlg();

		static void Show(const Curve::Key& key, const Function<void(const Curve::Key& key)>& onClosed);

	private:
		Ref<o2::Window> mWindow;

		Ref<EditBox> mPosition;
		Ref<EditBox> mValue;
		Ref<EditBox> mLeftSupportPosition;
		Ref<EditBox> mLeftSupportValue;
		Ref<EditBox> mRightSupportPosition;
		Ref<EditBox> mRightSupportValue;

		Function<void(const Curve::Key& key)> mOnClosed;

	private:
		// Called when Ok button pressed, calls onClosed function with new key parameters
		void OnOkPressed();
	};
}