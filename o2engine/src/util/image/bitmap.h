#pragma once

#include "public.h"
#include "util/math/color.h"
#include "util/math/vector2.h"
#include "util/smart_ptrs.h"

OPEN_O2_NAMESPACE

class LogStream;

/** Image. Containing formatted pixel data, size, filename. */
class Bitmap
{
public:
	enum class Format { Default = 0, R8G8B8A8 };
	enum class ImageType { Auto = 0, Png };

protected:
	Format mFormat;   /**< Image format. */
	uint8* mData;     /**< Data array. */
	Vec2I  mSize;     /**< Size of image, in pixels. */
	String mFilename; /**< File name. Empty if no file. */

public:
	Bitmap();
	Bitmap(Format format, const Vec2I& size);
	Bitmap(const String& fileName, ImageType type = ImageType::Auto, LogStream* clog = NULL);
	virtual ~Bitmap();

	/** Creates image with specified format. */
	void Create(Format format, const Vec2I& size);

	/** Loading image from file. */
	bool Load(const String& fileName, ImageType type = ImageType::Auto, LogStream* clog = NULL);

	/** Saving image to file. */
	bool Save(const String& fileName, ImageType type, LogStream* clog = NULL) const;

	/** Clearing image with color. */
	void Clear(const Color4& color);

	/** Returns data. */
	uint8* GetData();

	/** Returns const data. */
	const uint8* getData() const;

	/** Returns size of image. */
	Vec2I GetSize() const;

	/** Returns pixel format. */
	Format GetFormat() const;

	/** Return file name. */
	const String& GetFilename() const;

	/** Copy image to position. */
	void CopyImage(Bitmap* img, const Vec2I& position = Vec2I());

	/** Fill image with color. */
	void Fill(const Color4& color);
};

CLOSE_O2_NAMESPACE