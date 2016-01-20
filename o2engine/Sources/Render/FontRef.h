#pragma once

#include "Render/Font.h"

namespace o2
{
	// --------------
	// Font reference
	// --------------
	class FontRef
	{
	public:
		// Default constructor
		FontRef();

		// Constructor
		FontRef(Font* font);

		// Constructor
		FontRef(const String& fileName);

		// Copy-constructor
		FontRef(const FontRef& other);

		// Destructor
		~FontRef();

		// Assign operator
		FontRef& operator=(const FontRef& other);

		// Font pointer operator
		Font* operator->();

		// Constant font pointer operator
		const Font* operator->() const;

		// Reference access operator
		Font& operator*();

		// Returns object pointer
		Font* Get();

		// Returns constant object pointer
		const Font* Get() const;

		// Check equal operator
		bool operator==(const FontRef& other) const;

		// Check not equal operator
		bool operator!=(const FontRef& other) const;

		// Returns true if texture isn't null
		bool IsValid() const;

		// Returns true if texture isn't null
		operator bool() const;

	protected:
		Font* mFont; // Font pointer

		friend class Render;
		friend class VectorFontAsset;
	};
}

