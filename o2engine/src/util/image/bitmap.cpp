#include "bitmap.h"

#include "png_image_format.h"
#include "util\log.h"

OPEN_O2_NAMESPACE

Bitmap::Bitmap():
mFormat(Format::Default), mData(NULL)
{
}

Bitmap::Bitmap(Format format, const Vec2I& size):
mFormat(format), mSize(size), mData(NULL)
{
	Create(format, size);
}

Bitmap::Bitmap(const String& fileName, ImageType type /*= IT_AUTO*/, LogStream* clog /*= NULL*/):
mFormat(Format::Default), mData(NULL)
{
	Load(fileName, type, clog);
}

Bitmap::~Bitmap()
{
	SafeReleaseArr(mData);
}

void Bitmap::Create(Format format, const Vec2I& size)
{
	if (mData)
		SafeReleaseArr(mData);

	short bpp[] ={0, 4};

	mFormat = format;
	mSize = size;

	mData = mnew unsigned char[size.x*size.y*bpp[(int)format]];
}

bool Bitmap::Load(const String& fileName, ImageType type, LogStream* clog /*= NULL*/)
{
	LogStream* log = clog ? clog:gLog;

	mFilename = fileName;

	if (type == ImageType::Png)
		return LoadPngImage(fileName, this, true, log);
	else
	{
		if (LoadPngImage(fileName, this, false, log))
			return true;

		log->Error("Can't load image '%s': unknown format", fileName.c_str());
	}

	mFilename = "";

	return false;
}

bool Bitmap::Save(const String& fileName, ImageType type, LogStream* clog /*= NULL*/) const
{
	LogStream* log = clog ? clog:gLog;

	if (type == ImageType::Png || type == ImageType::Auto)
	{
		return SavePngImage(fileName, this, log);
	}

	log->Error("Can't save image to '%s': unknown format specified", fileName.c_str());

	return false;
}

void Bitmap::Clear(const Color4& color)
{
	short bpp[] ={0, 4};

	memset(mData, color.ARGB(), bpp[(int)mFormat]*mSize.x*mSize.y);
}

unsigned char* Bitmap::GetData()
{
	return mData;
}

Vec2I Bitmap::GetSize() const
{
	return mSize;
}

Bitmap::Format Bitmap::GetFormat() const
{
	return mFormat;
}

const unsigned char* Bitmap::getData() const
{
	return mData;
}

const String& Bitmap::GetFilename() const
{
	return mFilename;
}

void Bitmap::CopyImage(Bitmap* img, const Vec2I& position /*= vec2f()*/)
{
	if (mFormat != img->mFormat)
		return;

	int pixelSize = 4;

	for (int x = 0; x < img->mSize.x; x++)
	{
		if (x + position.x >= mSize.x)
			break;

		for (int y = 0; y < img->mSize.y; y++)
		{
			if (y + position.y >= mSize.y)
				break;

			uint srcIdx = (img->mSize.y - y - 1)*img->mSize.x + x;
			uint dstIdx = (mSize.y - 1 - (y + position.y))*mSize.x + x + position.x;

			unsigned long colr = Color4::Blue().AsULong();
			memcpy(mData + dstIdx*pixelSize, img->mData + srcIdx*pixelSize, pixelSize);
		}
	}
}

void Bitmap::Fill(const Color4& color)
{
	unsigned long colrDw = color.AsULong();
	for (int x = 0; x < mSize.x*mSize.y; x++)
		memcpy(mData + x*4, &colrDw, 4);
}

CLOSE_O2_NAMESPACE