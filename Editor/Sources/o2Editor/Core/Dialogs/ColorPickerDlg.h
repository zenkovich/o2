#pragma once

#include "o2/Events/CursorEventsArea.h"
#include "o2/Events/CursorAreaEventsListener.h"
#include "o2/Render/TextureRef.h"
#include "o2/Utils/Function/Function.h"
#include "o2/Utils/Math/Color.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Ref.h"
#include "o2/Utils/WeakRef.h"

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
        ColorPickerDlg();
        ~ColorPickerDlg();

        static void Show(const Color4& color, const Function<void(const Color4&)>& onChanged,
                         const Function<void()>& onCompleted = Function<void()>());

    protected:
        Function<void(const Color4&)> mOnChangedCallback;
        Function<void()> mOnCompletedCallback;

        Color4 mColorValue;
        ColorType mColorType = ColorType::RGB;

        Ref<o2::Window> mWindow;
        Ref<Image> mColorSampleImage;

        Ref<Texture> mChessBackTexture;

        Ref<Image> mColorPickAreaHandle;
        Ref<Bitmap> mColorPickAreaBitmap;
        Ref<WidgetLayer> mColorPickAreaColor;
        Ref<Texture> mColorPickAreaTexture;
        CursorEventsArea mColorPickHandle;

        Ref<VerticalProgress> mHUEBar;
        Ref<Bitmap> mHUEBarBitmap;
        Ref<Texture> mHUEBarTexture;

        Ref<DropDown> mTypeDropdown;

        Ref<Label> mColor1ParamName;
        Ref<HorizontalProgress> mColor1ParamBar;
        Ref<EditBox> mColor1ParamEdit;
        Ref<Bitmap> mColor1ParamBarBitmap;
        Ref<Texture> mColor1ParamBarTexture;

        Ref<Label> mColor2ParamName;
        Ref<HorizontalProgress> mColor2ParamBar;
        Ref<EditBox> mColor2ParamEdit;
        Ref<Bitmap> mColor2ParamBarBitmap;
        Ref<Texture> mColor2ParamBarTexture;

        Ref<Label> mColor3ParamName;
        Ref<HorizontalProgress> mColor3ParamBar;
        Ref<EditBox> mColor3ParamEdit;
        Ref<Bitmap> mColor3ParamBarBitmap;
        Ref<Texture> mColor3ParamBarTexture;

        Ref<Label> mColorAParamName;
        Ref<HorizontalProgress> mColorAParamBar;
        Ref<EditBox> mColorAParamEdit;
        Ref<Bitmap> mColorAParamBarBitmap;
        Ref<Texture> mColorAParamBarTexture;

    protected:
        void OnHide();
        void InitializeControls();
        void InitializeColorPreview();
        void InitializePickArea();
        void InitializeColorParams();

        Ref<Widget> InitializeColorParameter(Ref<Label>& name, Ref<HorizontalProgress>& bar, Ref<EditBox>& edit,
                                             Ref<Bitmap>& bitmap, Ref<Texture>& texture,
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

// --- META ---

PRE_ENUM_META(Editor::ColorPickerDlg::ColorType);
// --- END META ---