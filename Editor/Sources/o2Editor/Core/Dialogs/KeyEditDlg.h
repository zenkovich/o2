#pragma once

#include "o2/Utils/Singleton.h"

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
		Window*  mWindow;
		EditBox* mPosition;
		EditBox* mValue;
		EditBox* mLeftSupportPosition;
		EditBox* mLeftSupportValue;
		EditBox* mRightSupportPosition;
		EditBox* mRightSupportValue;

		Function<void(const Curve::Key& key)> mOnClosed;

	private:
		// It is called when Ok button pressed, calls onClosed function with new key parameters
		void OnOkPressed();
	};
}
