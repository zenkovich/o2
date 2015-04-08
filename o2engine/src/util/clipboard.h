#pragma once

#include "public.h"

OPEN_O2_NAMESPACE

//TODO: put these functions into static class

/** Sets the clipboard data text. */
void SetClipboardText(const String& text);

/** Returns clipboard data as text. */
String GetClipboardText();

CLOSE_O2_NAMESPACE
