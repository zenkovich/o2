#pragma once

#include "Utils/Types/String.h"

namespace o2
{
	class Bitmap;

	bool LoadPngImage(const String& fileName, Bitmap* image, bool errors = true);
	bool SavePngImage(const String& fileName, const Bitmap* image);
}
