#pragma once

#include "o2/Events/CursorEventsArea.h"
#include "o2/Events/CursorAreaEventsListener.h"
#include "o2/Render/TextureRef.h"
#include "o2/Utils/Function/Function.h"
#include "o2/Utils/Math/Color.h"
#include "o2/Utils/Singleton.h"

using namespace o2;

namespace o2
{
	class Bitmap;
	class DropDown;
	class EditBox;
	class HorizontalProgress;
	class Image;
	class Label;
	class VerticalProgress;
	class Widget;
	class WidgetLayer;
	class Window;
}

namespace Editor
{
	// --------------------
	// Color picking dialog
	// --------------------
	class ColorPickerDlg : public Singleton<ColorPickerDlg>, public CursorEventsListener
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

		Color4    mColorValue;
		ColorType mColorType = ColorType::RGB;

		o2::Window* mWindow = nullptr;
		Image*      mColorSampleImage = nullptr;

		TextureRef mChessBackTexture;

		Image*           mColorPickAreaHandle = nullptr;
		Bitmap*          mColorPickAreaBitmap = nullptr;
		WidgetLayer*     mColorPickAreaColor = nullptr;
		TextureRef       mColorPickAreaTexture;
		CursorEventsArea mColorPickHandle;

		VerticalProgress* mHUEBar = nullptr;
		Bitmap*           mHUEBarBitmap = nullptr;
		TextureRef        mHUEBarTexture;

		DropDown* mTypeDropdown = nullptr;

		Label*              mColor1ParamName = nullptr;
		HorizontalProgress* mColor1ParamBar = nullptr;
		EditBox*            mColor1ParamEdit = nullptr;
		Bitmap*             mColor1ParamBarBitmap = nullptr;
		TextureRef          mColor1ParamBarTexture;

		Label*              mColor2ParamName = nullptr;
		HorizontalProgress* mColor2ParamBar;
		EditBox*            mColor2ParamEdit = nullptr;
		Bitmap*             mColor2ParamBarBitmap = nullptr;
		TextureRef          mColor2ParamBarTexture;

		Label*              mColor3ParamName = nullptr;
		HorizontalProgress* mColor3ParamBar;
		EditBox*            mColor3ParamEdit = nullptr;
		Bitmap*             mColor3ParamBarBitmap = nullptr;
		TextureRef          mColor3ParamBarTexture;

		Label*              mColorAParamName = nullptr;
		HorizontalProgress* mColorAParamBar;
		EditBox*            mColorAParamEdit = nullptr;
		Bitmap*             mColorAParamBarBitmap = nullptr;
		TextureRef          mColorAParamBarTexture;


	protected:
		void OnHide();
		void InitializeControls();
		void InitializeColorPreview();
		void InitializePickArea();
		void InitializeColorParams();

		Widget* InitializeColorParameter(Label*& name, HorizontalProgress*& bar, EditBox*& edit,
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

PRE_ENUM_META(Editor::ColorPickerDlg::ColorType);
