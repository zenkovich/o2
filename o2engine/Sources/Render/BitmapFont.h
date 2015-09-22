#pragma once

#include "Render/Font.h"

namespace o2
{
	// -----------
	// Bitmap font
	// -----------
	class BitmapFont: public Font
	{
	public:
		// Default constructor
		BitmapFont();

		// Constructor with loading from file
		BitmapFont(const String& fileName);

		// Loads bitmap font data from file
		bool Load(const String& fileName);

		// Returns file name
		String GetFileName() const;

	protected:
		String mFileName; // Source file name
	};
}
