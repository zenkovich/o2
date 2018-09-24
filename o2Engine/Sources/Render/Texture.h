#pragma once

#if defined PLATFORM_WINDOWS
#include "Render/Windows/TextureBase.h"
#elif defined PLATFORM_ANDROID
#include "Render/Android/TextureBase.h"
#endif

#include "Utils/Math/Vector2.h"
#include "Utils/Property.h"
#include "Utils/Types/String.h"
#include "Utils/Types/UID.h"

namespace o2
{
	class Bitmap;
	class TextureRef;

	// -------
	// Texture
	// -------
	class Texture: public TextureBase
	{
	public:
		// Texture usage
		enum class Usage { Default, RenderTarget };

	public:
		PROPERTIES(Texture);
		GETTER(Vec2I,  size, GetSize);         // Size of texture getter
		GETTER(PixelFormat, format, GetFormat);     // texture format getter
		GETTER(Usage,  usage, GetUsage);       // Texture usage getter
		GETTER(String, fileName, GetFileName); // Texture file name getter

		// Default constructor
		Texture();

		// Constructor
		Texture(const Vec2I& size, PixelFormat format = PixelFormat::R8G8B8A8, Usage usage = Usage::Default);

		// Constructor from file
		Texture(const String& fileName);

		// Constructor from atlas page
		Texture(UID atlasAssetId, int page);

		// Constructor from atlas page
		Texture(const String& atlasAssetName, int page);

		// Constructor from bitmap
		Texture(Bitmap* bitmap);

		// Destructor
		~Texture();

		// Creates texture with size, format and usage
		void Create(const Vec2I& size, PixelFormat format = PixelFormat::R8G8B8A8, Usage usage = Usage::Default);

		// Creates texture from file
		void Create(const String& fileName);

		// Creates texture from atlas page
		void Create(UID atlasAssetId, int page);

		// Creates texture from atlas page
		void Create(const String& atlasAssetName, int page);

		// Creates texture from bitmap
		void Create(Bitmap* bitmap);

		// Sets texture's data from bitmap
		void SetData(Bitmap* bitmap);

		// Sets sub data from bitmap with offset
		void SetSubData(const Vec2I& offset, Bitmap* bitmap);

		// Copies pixels from texture by rect
		void Copy(const Texture& from, const RectI& rect);

		// Gets texture's data to bitmap and returns it
		Bitmap* GetData();

		// Reloads texture
		void Reload();

		// Returns size of texture
		Vec2I GetSize() const;

		// Returns format
		PixelFormat GetFormat() const;

		// returns texture usage
		Usage GetUsage() const;

		// Returns texture file name
		String GetFileName() const;

		// Returns true when texture ready to use
		bool IsReady() const;

		// Returns is texture from atlas page
		bool IsAtlasPage() const;

		// Returns atlas asset id
		UID GetAtlasAssetId() const;

		// Returns atlas page
		int GetAtlasPage() const;

	protected:
		typedef Vector<TextureRef*> TextureRefsVec;

		Vec2I          mSize;         // Size of texture
		PixelFormat         mFormat;       // Texture format
		Usage          mUsage;        // Texture usage
		String         mFileName;     // Source file name
		UID            mAtlasAssetId; // Atlas asset id. Equals 0 if it isn't atlas texture
		int            mAtlasPage;    // Atlas page
		bool           mReady;        // Is texture ready to use
		TextureRefsVec mRefs;         // Texture references

		friend class Render;
		friend class TextureRef;
	};
}
