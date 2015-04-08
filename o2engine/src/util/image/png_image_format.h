#pragma once

#include "public.h"

OPEN_O2_NAMESPACE

class Bitmap;
class LogStream;

bool LoadPngImage(const String& fileName, Bitmap* image, bool errors = true, LogStream* plog = NULL);
bool SavePngImage(const String& fileName, const Bitmap* image, LogStream* plog = NULL);

CLOSE_O2_NAMESPACE