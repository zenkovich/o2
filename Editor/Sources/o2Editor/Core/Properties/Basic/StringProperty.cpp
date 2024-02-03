#include "o2Editor/stdafx.h"
#include "StringProperty.h"

#include "o2/Scene/UI/Widgets/EditBox.h"
#include "o2/core/Ref.h"
#include "o2/core/WeakRef.h"

namespace Editor
{
	using namespace o2;

	class StringProperty : public TPropertyField<Ref<std::string>>
	{
		using Base = TPropertyField<Ref<std::string>>;

	public:
		StringProperty() {}

		StringProperty(const StringProperty& other) :
			Base(other)
		{
			InitializeControls();
		}

		StringProperty& operator=(const StringProperty& other)
		{
			Base::operator=(other);
			InitializeControls();
			return *this;
		}

		void InitializeControls()
		{
			mEditBox = FindChildByType<EditBox>();
			if (mEditBox)
			{
				mEditBox->onChangeCompleted = [this](const WString& data) {
					OnEdited(data);
				};
				mEditBox->text = "--";
			}
		}

		void UpdateValueView()
		{
			if (mValuesDifferent)
				mEditBox->text = "--";
			else
				mEditBox->text = mCommonValue;
		}

		void OnEdited(const WString& data)
		{
			if (mValuesDifferent && data == "--")
				return;

			SetValueByUser(data);
		}

	private:
		Ref<EditBox> mEditBox;
	};

	DECLARE_TEMPLATE_CLASS(Editor::TPropertyField<Ref<o2::String>>);
}

DECLARE_CLASS(Editor::StringProperty, Editor__StringProperty);
// --- END META ---