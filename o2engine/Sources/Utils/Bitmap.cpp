#include "Bitmap.h"

#include "Utils/Debug.h"
#include "Utils/ImageFormats/PngFormat.h"

namespace o2
{
	Bitmap::Bitmap():
		mFormat(Format::Default), mData(NULL)
	{
		InitializeProperties();
	}

	Bitmap::Bitmap(Format format, const Vec2I& size):
		mFormat(format), mSize(size), mData(NULL)
	{
		InitializeProperties();
		Create(format, size);
	}

	Bitmap::Bitmap(const String& fileName, ImageType type /*= IT_AUTO*/):
		mFormat(Format::Default), mData(NULL)
	{
		InitializeProperties();
		Load(fileName, type);
	}

	Bitmap::Bitmap(const Bitmap& other)
	{
		InitializeProperties();

		short bpp[] ={0, 4};

		mFormat = other.mFormat;
		mSize = other.mSize;

		UInt dataSize = mSize.x*mSize.y*bpp[(int)mFormat];
		mData = mnew unsigned char[dataSize];
		memcpy(mData, other.mData, dataSize);
	}

	Bitmap::~Bitmap()
	{
		if (mData)
			delete[] mData;
	}

	Bitmap& Bitmap::operator=(const Bitmap& other)
	{
		if (mData)
			delete[] mData;

		short bpp[] ={0, 4};

		mFormat = other.mFormat;
		mSize = other.mSize;

		UInt dataSize = mSize.x*mSize.y*bpp[(int)mFormat];
		mData = mnew unsigned char[dataSize];
		memcpy(mData, other.mData, dataSize);

		return *this;
	}

	Ptr<Bitmap> Bitmap::Clone() const
	{
		return mnew Bitmap(*this);
	}

	void Bitmap::Create(Format format, const Vec2I& size)
	{
		if (mData)
			delete[] mData;

		short bpp[] = {0, 4};

		mFormat = format;
		mSize = size;

		mData = mnew unsigned char[size.x*size.y*bpp[(int)format]];
	}

	bool Bitmap::Load(const String& fileName, ImageType type)
	{
		mFilename = fileName;

		if (type == ImageType::Png)
			return LoadPngImage(fileName, this, true);
		else
		{
			if (LoadPngImage(fileName, this, false))
				return true;

			o2Debug.LogError("Can't load image '%s': unknown format", fileName);
		}

		mFilename = "";

		return false;
	}

	bool Bitmap::Save(const String& fileName, ImageType type) const
	{
		if (type == ImageType::Png || type == ImageType::Auto)
		{
			return SavePngImage(fileName, this);
		}

		o2Debug.LogError("Can't save image to '%s': unknown format specified", fileName);

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

	void Bitmap::CopyImage(Ptr<Bitmap> img, const Vec2I& position /*= Vec2I()*/)
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

				UInt srcIdx = (img->mSize.y - y - 1)*img->mSize.x + x;
				UInt dstIdx = (mSize.y - 1 - (y + position.y))*mSize.x + x + position.x;

				unsigned long colr = Color4::Blue().ARGB();
				memcpy(mData + dstIdx*pixelSize, img->mData + srcIdx*pixelSize, pixelSize);
			}
		}
	}

	void Bitmap::Fill(const Color4& color)
	{
		unsigned long colrDw = color.ARGB();

		for (int x = 0; x < mSize.x*mSize.y; x++)
			memcpy(mData + x*4, &colrDw, 4);
	}

	void Bitmap::InitializeProperties()
	{
		INITIALIZE_GETTER(Bitmap, data, GetData);
		INITIALIZE_GETTER(Bitmap, size, GetSize);
		INITIALIZE_GETTER(Bitmap, format, GetFormat);
	}

}