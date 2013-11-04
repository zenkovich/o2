#ifndef IMAGE_H
#define IMAGE_H

#include "public.h"
#include "util/math/vector2.h"
#include "util/math/color.h"

OPEN_O2_NAMESPACE

class cLogStream;

/** Image. Containing formatted pixel data, size, filename. */
class cImage
{
public:
	enum Format { FMT_NONE = 0, FMT_R8G8B8A8 };
	enum ImageType { IT_AUTO = 0, IT_PNG };

protected:
	Format         mFormat;   /**< Image format. */
	unsigned char* mData;     /**< Data array. */
	vec2i          mSize;     /**< Size of image, in pixels. */
	std::string    mFilename; /**< File name. Empty if no file. */

public:
	cImage();
	cImage(Format format, const vec2i& size);
	virtual ~cImage();

	/** Creates image with specified format. */
	void create(Format format, const vec2i& size);

	/** Loading image from file. */
	bool load(const std::string& fileName, ImageType type = IT_AUTO, cLogStream* clog = NULL);

	/** Saving image to file. */
	bool save(const std::string& fileName, ImageType type, cLogStream* clog = NULL) const;

	/** Clearing image with color. */
	void clear(const color4& color);

	/** Returns data. */
	unsigned char* getData();

	/** Returns const data. */
	const unsigned char* getDataConst() const;

	/** Returns size of image. */
	vec2i getSize() const;

	/** Returns pixel format. */
	Format getFormat() const;

	/** Return file name. */
	const std::string& getFilename() const;
};

CLOSE_O2_NAMESPACE

#endif //IMAGE_H