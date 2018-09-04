#pragma once

#include "Render/Texture.h"

namespace o2
{
	class TextureRef
	{
	public:
		// Default constructor
		TextureRef();

		// Constructor
		TextureRef(Texture* texture);

		// Constructor
		TextureRef(const Vec2I& size, 
				   PixelFormat format = PixelFormat::R8G8B8A8,
				   Texture::Usage usage = Texture::Usage::Default);

		// Constructor from file
		TextureRef(const String& fileName);

		// Constructor from bitmap
		TextureRef(Bitmap* bitmap);

		// Constructor from atlas page
		TextureRef(UID atlasAssetId, int page);

		// Constructor from atlas page
		TextureRef(const String& atlasAssetName, int page);

		// Copy-constructor
		TextureRef(const TextureRef& other);

		// Destructor
		~TextureRef();

		// Assign operator
		TextureRef& operator=(const TextureRef& other);

		// Texture pointer operator
		Texture* operator->();

		// Constant texture pointer operator
		const Texture* operator->() const;

		// Check equal operator
		bool operator==(const TextureRef& other) const;

		// Check not equal operator
		bool operator!=(const TextureRef& other) const;

		// Returns true if texture isn't null
		bool IsValid() const;

		// Returns true if texture isn't null
		operator bool() const;

		static TextureRef Null();

	protected:
		Texture* mTexture;

		friend class Render;
		friend class Texture;
	};

	// Returns reference to null texture
	TextureRef NoTexture();
}
