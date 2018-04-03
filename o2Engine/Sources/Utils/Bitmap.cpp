#include "stdafx.h"
#include "Bitmap.h"

#include "Utils/Debug.h"
#include "Utils/ImageFormats/PngFormat.h"
#include "Utils/Reflection/Reflection.h"

namespace o2
{
	Bitmap::Bitmap():
		mFormat(Format::Default), mData(nullptr)
	{}

	Bitmap::Bitmap(Format format, const Vec2I& size) :
		mFormat(format), mSize(size), mData(nullptr)
	{
		Create(format, size);
	}

	Bitmap::Bitmap(const String& fileName, ImageType type /*= IT_AUTO*/):
		mFormat(Format::Default), mData(nullptr)
	{
		Load(fileName, type);
	}

	Bitmap::Bitmap(const Bitmap& other):
		data(this), size(this), format(this)
	{
		short bpp[] ={ 0, 4 };

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

		short bpp[] ={ 0, 4 };

		mFormat = other.mFormat;
		mSize = other.mSize;

		UInt dataSize = mSize.x*mSize.y*bpp[(int)mFormat];
		mData = mnew unsigned char[dataSize];
		memcpy(mData, other.mData, dataSize);

		return *this;
	}

	Bitmap* Bitmap::Clone() const
	{
		return mnew Bitmap(*this);
	}

	void Bitmap::Create(Format format, const Vec2I& size)
	{
		if (mData)
			delete[] mData;

		short bpp[] ={ 0, 4 };

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
		Fill(color);
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

	void Bitmap::CopyImage(Bitmap* img, const Vec2I& position /*= Vec2I()*/, const RectI& imgSrc /*= RectI()*/)
	{
		if (mFormat != img->mFormat)
			return;

		RectI imgSrcRect = imgSrc;

		if (imgSrcRect.Width() == 0)
			imgSrcRect.Set(Vec2I(), img->GetSize());

		int bpp[] ={ 0, 4 };
		int curbpp = bpp[(int)mFormat];
		int pixelSize = curbpp;

		for (int x = 0; x < imgSrcRect.right - imgSrcRect.left; x++)
		{
			if (x + position.x >= mSize.x)
				break;

			for (int y = 0; y < imgSrcRect.top - imgSrcRect.bottom; y++)
			{
				if (y + position.y >= mSize.y)
					break;

				UInt srcIdx = (img->mSize.y - (y + imgSrcRect.bottom) - 1)*img->mSize.x + x + imgSrcRect.left;
				UInt dstIdx = (mSize.y - 1 - (y + position.y))*mSize.x + x + position.x;

				memcpy(mData + dstIdx*pixelSize, img->mData + srcIdx*pixelSize, pixelSize);
			}
		}
	}

	void Bitmap::BlendImage(Bitmap* img, const Vec2I& position /*= Vec2I()*/, const RectI& imgSrc /*= RectI()*/)
	{
		if (mFormat != img->mFormat)
			return;

		RectI imgSrcRect = imgSrc;

		if (imgSrcRect.Width() == 0)
			imgSrcRect.Set(Vec2I(), img->GetSize());

		int bpp[] ={ 0, 4 };
		int curbpp = bpp[(int)mFormat];
		int pixelSize = curbpp;

		for (int x = 0; x < imgSrcRect.right - imgSrcRect.left; x++)
		{
			if (x + position.x >= mSize.x)
				break;

			for (int y = 0; y < imgSrcRect.top - imgSrcRect.bottom; y++)
			{
				if (y + position.y >= mSize.y)
					break;

				UInt srcIdx = (img->mSize.y - (y + imgSrcRect.bottom) - 1)*img->mSize.x + x + imgSrcRect.left;
				UInt dstIdx = (mSize.y - 1 - (y + position.y))*mSize.x + x + position.x;

				Color4 srcColor, src1Color;
				srcColor.SetABGR(*(ULong*)(mData + dstIdx*pixelSize));
				src1Color.SetABGR(*(ULong*)(img->mData + srcIdx*pixelSize));

				Color4 resColor = srcColor.BlendByAlpha(src1Color);
				ULong uresColor = resColor.ABGR();

				memcpy(mData + dstIdx*pixelSize, &uresColor, pixelSize);
			}
		}
	}

	void Bitmap::Colorise(const Color4& color)
	{
		int bpp[] ={ 0, 4 };
		int curbpp = bpp[(int)mFormat];

		for (int x = 0; x < mSize.x*mSize.y; x++)
		{
			Color4 c;
			c.SetABGR(*(ULong*)(mData + x*curbpp));
			c *= color;
			*(ULong*)(mData + x*curbpp) = c.ABGR();
		}
	}

	void Bitmap::GradientByAlpha(const Color4& color1, const Color4& color4, float angle /*= 0*/, float size /*= 0*/,
								 Vec2F origin /*= Vec2F()*/)
	{
		int bpp[] ={ 0, 4 };
		int curbpp = bpp[(int)mFormat];

		Vec2F dir = Vec2F::Rotated(Math::Deg2rad(angle + 90.0f));
		if (Math::Equals(size, 0))
			size = ((Vec2F)mSize).Dot(dir);

		float invSize = 1.0f / size;

		Vec2F pxorigin = origin*(Vec2F)mSize;

		for (int x = 0; x < mSize.x; x++)
		{
			for (int y = 0; y < mSize.y; y++)
			{
				Vec2F p = Vec2F((float)x, (float)y) - pxorigin;
				float proj = p.Dot(dir);
				float coef = Math::Clamp01(proj*invSize);
				ULong offs = (y*mSize.x + x)*curbpp;

				Color4 c;
				c.SetABGR(*(ULong*)(mData + offs));
				c *= Math::Lerp(color1, color4, coef);
				*(ULong*)(mData + offs) = c.ABGR();
			}
		}
	}

	void Bitmap::Fill(const Color4& color)
	{
		unsigned long colrDw = color.ARGB();
		int bpp[] ={ 0, 4 };
		int curbpp = bpp[(int)mFormat];

		for (int x = 0; x < mSize.x*mSize.y; x++)
			memcpy(mData + x*curbpp, &colrDw, curbpp);
	}

	void Bitmap::FillRect(int rtLeft, int rtTop, int rtRight, int rtBottom, const Color4& color)
	{
		unsigned long colrDw = color.ARGB();
		int bpp[] ={ 0, 4 };
		int curbpp = bpp[(int)mFormat];

		for (int x = Math::Max(rtLeft, 0); x < Math::Min(mSize.x, rtRight); x++)
		{
			for (int y = Math::Max(rtTop, 0); y < Math::Min(mSize.y, rtBottom); y++)
				memcpy(&mData[(y*mSize.x + x)*curbpp], &colrDw, curbpp);
		}
	}

	void Bitmap::Blur(float radius)
	{
		int mapSize = Math::CeilToInt(radius);
		int fullmapSize = mapSize * 2 + 1;

		float** weightMap = mnew float*[fullmapSize];
		for (int i = 0; i < fullmapSize; i++)
		{
			weightMap[i] = mnew float[fullmapSize];

			for (int j = 0; j < fullmapSize; j++)
			{
				float x = (float)(i - mapSize), y = (float)(j - mapSize);
				float dst = Math::Sqrt(x*x + y*y);
				weightMap[i][j] = Math::Clamp01(1.0f - dst / radius);
			}
		}

		int bpp[] ={ 0, 4 };
		int curbpp = bpp[(int)mFormat];
		UInt8* srcData = mnew UInt8[mSize.x*mSize.y*curbpp];
		memcpy(srcData, mData, mSize.x*mSize.y*curbpp);

		for (int x = 0; x < mSize.x; x++)
		{
			for (int y = 0; y < mSize.y; y++)
			{
				Color4 c;
				Color4 csum(0, 0, 0, 0);
				float wSum = 0;

				for (int ox = 0; ox < fullmapSize; ox++)
				{
					for (int oy = 0; oy < fullmapSize; oy++)
					{
						int cx = x + (ox - mapSize), cy = y + (oy - mapSize);

						if (cx < 0 || cx >= mSize.x || cy < 0 || cy >= mSize.y)
							continue;

						c.SetARGB(*(ULong*)&srcData[(cy*mSize.x + cx)*curbpp]);
						float w = weightMap[ox][oy];
						csum += c*w;
						wSum += w;
					}
				}

				csum /= wSum;
				ULong ucsum = csum.ARGB();
				memcpy(&mData[(y*mSize.x + x)*curbpp], &ucsum, curbpp);
			}
		}

		for (int i = 0; i < fullmapSize; i++)
			delete[] weightMap[i];

		delete[] weightMap;
		delete[] srcData;
	}

	void Bitmap::Outline(float radius, const Color4& color, int threshold /*= 100*/)
	{
		int mapSize = Math::CeilToInt(radius);
		int fullmapSize = mapSize * 2 + 1;
		int alphaThreshold = threshold;
		int radiusSquare = Math::Sqr(Math::FloorToInt(radius));

		int bpp[] ={ 0, 4 };
		int curbpp = bpp[(int)mFormat];

		UInt8* srcData = mnew UInt8[mSize.x*mSize.y*curbpp];
		memcpy(srcData, mData, mSize.x*mSize.y*curbpp);

		for (int x = 0; x < mSize.x; x++)
		{
			for (int y = 0; y < mSize.y; y++)
			{
				float sqrDist = 0;
				int count = 0;
				Color4 pc;
				UInt offs = (y*mSize.x + x)*curbpp;
				pc.SetABGR(*(ULong*)&srcData[offs]);

				for (int ox = 0; ox < fullmapSize; ox++)
				{
					for (int oy = 0; oy < fullmapSize; oy++)
					{
						int fx = ox - mapSize, fy = oy - mapSize;
						int cx = x + (ox - mapSize), cy = y + (oy - mapSize);

						if (cx < 0 || cx >= mSize.x || cy < 0 || cy >= mSize.y)
							continue;

						Color4 c;
						c.SetABGR(*(ULong*)&srcData[(cy*mSize.x + cx)*curbpp]);
						if (c.a > alphaThreshold)
						{
							int dst = fx*fx + fy*fy;
							sqrDist += dst;
							count++;
						}
					}
				}

				sqrDist /= (float)count;

				if (sqrDist < radiusSquare - 2 && false)
				{
					Color4 newColor = pc.BlendByAlpha(color);
					ULong unewColor = newColor.ABGR();
					memcpy(&mData[offs], &unewColor, curbpp);

				}
				else
				{
					float distance = Math::Sqrt((float)sqrDist);

					if (distance < radius + 1.0f)
					{
						float alpha = 1.0f - (distance - radius);

						Color4 newColor = color;
						newColor.a = (int)((float)newColor.a*alpha);
						newColor = pc.BlendByAlpha(color);
						ULong unewColor = newColor.ABGR();
						memcpy(&mData[offs], &unewColor, curbpp);
					}
				}
			}
		}

		delete[] srcData;
	}
}

ENUM_META_(o2::Bitmap::Format, Format)
{
	ENUM_ENTRY(Default);
	ENUM_ENTRY(R8G8B8A8);
}
END_ENUM_META;

ENUM_META_(o2::Bitmap::ImageType, ImageType)
{
	ENUM_ENTRY(Auto);
	ENUM_ENTRY(Png);
}
END_ENUM_META;
