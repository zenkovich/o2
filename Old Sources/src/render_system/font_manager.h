#pragma once

#include "public.h"
#include "util/containers/dictionary.h"

OPEN_O2_NAMESPACE

class Font;
class RenderSystem;

/** Font manager. Containing set of fonts, loading/unloading fonts. */
class FontManager
{
	friend class RenderSystemBaseInterface;

public:
	typedef Dictionary< String, Font* > FontsDictionary;

protected:
	FontsDictionary mFonts; /** Fonts map. */

	/** ctor. */
	FontManager();

	/** dtor. */
	~FontManager();

public:
	/** Loading fonts set from file. */
	void LoadFontsSet(const String& fontsSetFile);

	/** Loading font from file. */
	Font* LoadFont(const String& fontFile);

	/** Loading BMFont from file. */
	Font* LoadBMFont(const String& fontFile);

	/** Unloading all fonts. */
	void UnloadFonts();

	/** Returns font by name. */
	Font* GetFont(const String& name);
};

CLOSE_O2_NAMESPACE
