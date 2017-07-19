#pragma once

#include "Events/CursorEventsListener.h"
#include "Render/TextureRef.h"
#include "Utils/CursorEventsArea.h"
#include "Utils/Delegates.h"
#include "Utils/Math/Color.h"
#include "Utils/Singleton.h"

using namespace o2;

namespace o2
{
	class Bitmap;
	class UIDropDown;
	class UIEditBox;
	class UIHorizontalProgress;
	class UIImage;
	class UILabel;
	class UIVerticalProgress;
	class UIWidget;
	class UIWidgetLayer;
	class UIWindow;
}

namespace Editor
{
	// --------------------
	// Color picking dialog
	// --------------------
	class ColorPickerDlg: public Singleton<ColorPickerDlg>, public CursorEventsListener
	{
	public:
		enum  class ColorType { RGB, HSL };

	public:
		ColorPickerDlg();
		~ColorPickerDlg();

		static void Show(const Color4& color, Function<void(const Color4&)> onChanged, 
						 Function<void()> onCompleted = Function<void()>());

	protected:
		Function<void(const Color4&)> mOnChangedCallback;
		Function<void()>              mOnCompletedCallback;

		Color4                mColorValue;
		ColorType             mColorType = ColorType::RGB;

		UIWindow*             mWindow;
		UIImage*              mColorSampleImage;

		TextureRef            mChessBackTexture;

		UIImage*              mColorPickAreaHandle;
		Bitmap*               mColorPickAreaBitmap;
		UIWidgetLayer*        mColorPickAreaColor;
		TextureRef            mColorPickAreaTexture;
		CursorEventsArea      mColorPickHandle;

		UIVerticalProgress*   mHUEBar;
		Bitmap*               mHUEBarBitmap;
		TextureRef            mHUEBarTexture;

		UIDropDown*           mTypeDropdown;

		UILabel*              mColor1ParamName;
		UIHorizontalProgress* mColor1ParamBar;
		UIEditBox*            mColor1ParamEdit;
		Bitmap*               mColor1ParamBarBitmap;
		TextureRef            mColor1ParamBarTexture;

		UILabel*              mColor2ParamName;
		UIHorizontalProgress* mColor2ParamBar;
		UIEditBox*            mColor2ParamEdit;
		Bitmap*               mColor2ParamBarBitmap;
		TextureRef            mColor2ParamBarTexture;

		UILabel*              mColor3ParamName;
		UIHorizontalProgress* mColor3ParamBar;
		UIEditBox*            mColor3ParamEdit;
		Bitmap*               mColor3ParamBarBitmap;
		TextureRef            mColor3ParamBarTexture;

		UILabel*              mColorAParamName;
		UIHorizontalProgress* mColorAParamBar;
		UIEditBox*            mColorAParamEdit;
		Bitmap*               mColorAParamBarBitmap;
		TextureRef            mColorAParamBarTexture;


	protected:
		void OnHide();
		void InitializeControls();
		void InitializeColorPreview();
		void InitializePickArea();
		void InitializeColorParams();
		UIWidget* InitializeColorParameter(UILabel*& name, UIHorizontalProgress*& bar, UIEditBox*& edit, 
										   Bitmap*& bitmap, TextureRef& texture, 
										   const Function<void(float)>& onChanged);

		void InitHUEBarBitmap();

		void OnColorPickHandleMoved(const Input::Cursor& cursor);
		void OnColorTypeSelected(const WString& name);

		void OnColor1ParameterEdited(float value);
		void OnColor2ParameterEdited(float value);
		void OnColor3ParameterEdited(float value);
		void OnColorAParameterEdited(float value);
		void OnHUEEdited(float value);

		void UpdateHandlesBitmaps();
		void UpdateHandlesValues();

		void UpdateRGBABitmaps();
		void UpdateHSLABitmaps();
		void UpdateColorPickBitmap();

		void OnCursorPressedOutside();
	};
}
