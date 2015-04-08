#include "font_manager.h"

#include "font.h"
#include "render_system.h"
#include "util/log.h"
#include "util/xml_tools.h"

OPEN_O2_NAMESPACE

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
	UnloadFonts();
}

void FontManager::LoadFontsSet(const String& fontsSetFile)
{
	pugi::xml_document doc;
	if (!XmlTools::LoadFromFile(fontsSetFile, doc))
	{
		AppRender()->mLog->Error("Failed to load font set file: %s", fontsSetFile.c_str());
		return;
	}

	for (pugi::xml_node obj = doc.child("Font"); obj; obj = obj.next_sibling("Font"))
	{
		LoadFont(obj.attribute("fileName").value());
	}
}

Font* FontManager::LoadFont(const String& fontFile)
{
	Font* newFont = mnew Font;
	newFont->Create(fontFile);
	mFonts[fontFile] = newFont;
	return newFont;
}

Font* FontManager::LoadBMFont(const String& fontFile)
{
	Font* newFont = mnew Font;
	newFont->CreateFromBMFont(fontFile);
	mFonts[fontFile] = newFont;
	return newFont;
}

void FontManager::UnloadFonts()
{
	for (FontsDictionary::iterator it = mFonts.begin(); it != mFonts.end(); ++it)
		SafeRelease(it->second);

	mFonts.clear();
}

Font* FontManager::GetFont(const String& name)
{
	return mFonts.at(name);
}

CLOSE_O2_NAMESPACE