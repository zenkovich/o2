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
        enum class ColorType { RGB, HSL };

    public:
        // Default constructor
        ColorPickerDlg(RefCounter* refCounter);

        // Destructor
        ~ColorPickerDlg();

        // Shows color picking window. Delegate onChanged is called when color changed and onCompleted when ok pressed
        static void Show(const Color4& color, const Function<void(const Color4&)>& onChanged,
                         const Function<void()>& onCompleted = {});

        REF_COUNTERABLE_IMPL(Singleton<ColorPickerDlg>, CursorEventsListener);

    protected:
        Function<void(const Color4&)> mOnChangedCallback;   // On changed callback
        Function<void()>              mOnCompletedCallback; // On completed callback

        Color4    mColorValue;                 // Current color value
        ColorType mColorType = ColorType::RGB; // Current color controls type

        Ref<o2::Window> mWindow;
        Ref<Image>      mColorSampleImage;

        TextureRef mChessBackTexture;

		Ref<Image>            mColorPickAreaHandle;
		Ref<Bitmap>           mColorPickAreaBitmap;
		Ref<WidgetLayer>      mColorPickAreaColor;
		TextureRef            mColorPickAreaTexture;
        Ref<CursorEventsArea> mColorPickHandle;

        Ref<VerticalProgress> mHUEBar;
        Ref<Bitmap>           mHUEBarBitmap;
        TextureRef            mHUEBarTexture;

        Ref<DropDown> mTypeDropdown;

        Ref<Label>              mColor1ParamName;
        Ref<HorizontalProgress> mColor1ParamBar;
        Ref<EditBox>            mColor1ParamEdit;
        Ref<Bitmap>             mColor1ParamBarBitmap;
        TextureRef              mColor1ParamBarTexture;

        Ref<Label>              mColor2ParamName;
        Ref<HorizontalProgress> mColor2ParamBar;
        Ref<EditBox>            mColor2ParamEdit;
        Ref<Bitmap>             mColor2ParamBarBitmap;
        TextureRef              mColor2ParamBarTexture;

        Ref<Label>              mColor3ParamName;
        Ref<HorizontalProgress> mColor3ParamBar;
        Ref<EditBox>            mColor3ParamEdit;
        Ref<Bitmap>             mColor3ParamBarBitmap;
        TextureRef              mColor3ParamBarTexture;

        Ref<Label>              mColorAParamName;
        Ref<HorizontalProgress> mColorAParamBar;
        Ref<EditBox>            mColorAParamEdit;
        Ref<Bitmap>             mColorAParamBarBitmap;
        TextureRef              mColorAParamBarTexture;


    protected:
        // Called when color picking dialog wehe hidden
        void OnHide();

        // Initializes all controls
        void InitializeControls();

        // Initializes color preview
        void InitializeColorPreview();

        // Initializes color pick area
        void InitializePickArea();

        // Initializes HUE/RGB bars
        void InitializeColorParams();

        // Initializes color parameter controls
        Ref<Widget> InitializeColorParameter(Ref<Label>& name, Ref<HorizontalProgress>& bar, Ref<EditBox>& edit,
                                             Ref<Bitmap>& bitmap, TextureRef& texture,
                                             const Function<void(float)>& onChanged);

        // Initializes HUE bar bitmap
        void InitHUEBarBitmap();

        // Called when color pick handle moved, updates actual color value
        void OnColorPickHandleMoved(const Input::Cursor& cursor);

        // Called when controls type changed, updates controls
        void OnColorTypeSelected(const WString& name);

        // Called when color parameter 1 (R/H) changed, updates actual color value
        void OnColor1ParameterEdited(float value);

        // Called when color parameter 2 (G/S) changed, updates actual color value
        void OnColor2ParameterEdited(float value);

        // Called when color parameter 3 (B/L) changed, updates actual color value
        void OnColor3ParameterEdited(float value);

        // Called when alpha changed, updates actual color value
        void OnColorAParameterEdited(float value);

        // Called when HUE changed, updates actual color value
        void OnHUEEdited(float value);

        // Called when color changed, updates color bars
        void UpdateHandlesBitmaps();

        // Updates color handles values by actual color value
        void UpdateHandlesValues();

        // Updates RGBA bitmaps
        void UpdateRGBABitmaps();

        // Updates HSLA bitmaps
        void UpdateHSLABitmaps();

        // Updates color pick bitmap
        void UpdateColorPickBitmap();

        // Called when cursor pressed outside of color picking dialog, closes it
        void OnCursorPressedOutside();
    };
}
// --- META ---

PRE_ENUM_META(Editor::ColorPickerDlg::ColorType);
// --- END META ---
