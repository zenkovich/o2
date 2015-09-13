#pragma once

#include "Render/Windows/TextureBase.h"

#include "Utils/Math/Vector2.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Property.h"
#include "Utils/String.h"

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
		// Texture format
		enum class Format { Default, R8G8B8A8, R8G8B8 };

		// Texture usage
		enum class Usage { Default, RenderTarget };

	public:
		Getter<Vec2I>  Size;      // Size of texture getter
		Getter<Format> TexFormat; // texture format getter
		Getter<Usage>  TexUsage;  // Texture usage getter
		Getter<String> FileName;  // Texture file name getter

		// Default constructor
		Texture();

		// Constructor
		Texture(const Vec2I& size, Format format = Format::Default, Usage usage = Usage::Default);

		// Constructor from file
		Texture(const String& fileName);

		// Constructor from bitmap
		Texture(Ptr<Bitmap> bitmap);

		// Destructor
		~Texture();

		// Creates texture with size, format and usage
		void Create(const Vec2I& size, Format format = Format::Default, Usage usage = Usage::Default);

		// Creates texture from file
		void Create(const String& fileName);

		// Creates texture from bitmap
		void Create(Ptr<Bitmap> bitmap);

		// Returns size of texture
		Vec2I GetSize() const;

		// Returns format
		Format GetFormat() const;

		// returns texture usage
		Usage GetUsage() const;

		// Returns texture file name
		String GetFileName() const;

		// Returns true when texture ready to use
		bool IsReady() const;

	protected:
		typedef Vector<TextureRef*> TextureRefsVec;

		Vec2I          mSize;     // Size of texture
		Format         mFormat;   // Texture format
		Usage          mUsage;    // Texture usage
		String         mFileName; // Source file name
		bool           mReady;    // Is texture ready to use
		TextureRefsVec mRefs;     // Texture references

	protected:
		// Initializes properties
		void InitializeProperties();

		friend class Render;
		friend class TextureRef;
	};
}
