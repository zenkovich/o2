#pragma once

#include "public.h"
#include "ui_skin.h"

OPEN_O2_NAMESPACE

class Font;

class UIStdSkinInitializer
{
	static UISkinManager* mSkinManager;
	static Font*        mStdFont;

public:
	static void Initialize();
	static void Deinitialize();

protected:
	static void InitButton();
	static void InitBackground();
	static void InitProgressBar();
	static void InitCheckBox();
	static void InitHorScrollBar();
	static void InitHorThinScrollBar();
	static void InitVerThinScrollBar();
	static void InitSingleLineEditBox();
	static void InitLabel();
	static void InitMultilineEditBox();
	static void InitScrollArea();
	static void InitHint();
	static void InitRectPad();
};

CLOSE_O2_NAMESPACE
