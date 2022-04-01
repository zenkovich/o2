#pragma once

#include "o2/Render/Font.h"
#include "o2/Utils/Basic/IObject.h"

namespace o2
{
	// --------------
	// Font reference
	// --------------
	class FontRef: public IObject
	{
		IOBJECT(FontRef);

	public:
		// Default constructor
		FontRef();

		// Constructor
		FontRef(Font* font);

		// Constructor @SCRIPTABLE
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

		// Returns true if texture isn't null @SCRIPTABLE
		bool IsValid() const;

		// Returns true if texture isn't null
		operator bool() const;

	protected:
		Font* mFont; // Font pointer

		friend class Render;
		friend class VectorFontAsset;
	};
}

CLASS_BASES_META(o2::FontRef)
{
	BASE_CLASS(o2::IObject);
}
END_META;
CLASS_FIELDS_META(o2::FontRef)
{
	FIELD().PROTECTED().NAME(mFont);
}
END_META;
CLASS_METHODS_META(o2::FontRef)
{

	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PUBLIC().CONSTRUCTOR(Font*);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().CONSTRUCTOR(const String&);
	FUNCTION().PUBLIC().CONSTRUCTOR(const FontRef&);
	FUNCTION().PUBLIC().SIGNATURE(Font*, Get);
	FUNCTION().PUBLIC().SIGNATURE(const Font*, Get);
	FUNCTION().PUBLIC().SCRIPTABLE_ATTRIBUTE().SIGNATURE(bool, IsValid);
}
END_META;
