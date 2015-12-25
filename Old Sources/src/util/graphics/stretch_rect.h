#pragma once

#include "public.h"
#include "render_system/mesh.h"
#include "render_system/texture.h"
#include "util/graphics/rect_drawable.h"
#include "util/property.h"
#include "util/serialize_util.h"

OPEN_O2_NAMESPACE

class RenderSystem;

/** Stretching rect drawable. Contains some parts, what building using position and size of drawable. */
class StretchRect: public IRectDrawable
{
	/** Drawable part. Contains texture source rect, each corners colors, texture wrapping and position params. */
	struct Part
	{
		Vec2F  mLTPosPercent;    /** Left top corner relative position component. */
		Vec2F  mLTPosPixel;      /** Left top corner pixel position component. */
		Vec2F  mRBPosPercent;    /** Right down corner relative position component. */
		Vec2F  mRBPosPixel;      /** Right down corner pixel position component. */
		RectF  mTextureSrcRect;  /** Texture source rect. */
		bool   mWrapTexture;     /** True, if texture will be repeat and keep source rect size. */
		Color4 mVertexColors[4]; /** Corners colors. */
		bool   mClampLeft;       /** True, if need clamp left side by rect. */
		bool   mClampRight;      /** True, if need clamp right side by rect. */
		bool   mClampTop;        /** True, if need clamp top side by rect. */
		bool   mClampBottom;     /** True, if need clamp bottom side by rect. */

		/** def ctor. */
		Part();

		/** ctor. */
		Part(const Vec2F& LTPercent, const Vec2F& LTPixel, const Vec2F& RBPercent, const Vec2F& RBPixel,
			 const RectF& texRect, bool wrapTexture = false,
			 const Color4& vertex0Color = Color4::White(), const Color4& vertex1Color = Color4::White(),
			 const Color4& vertex2Color = Color4::White(), const Color4& vertex3Color = Color4::White());
	};
	typedef Array<Part> PartsArr;

	Mesh*    mMesh;             /** Mesh. */
	PartsArr mParts;            /** Parts array. */
	Vec2F    mMinSize;          /** Minimal size, when part will bot be clamped. */
	bool     mNeedUpdateMesh;   /** True, if need to update mesh verticies. */
	bool     mNeedUpdateColors; /** True, if need update just verticies colors. */

public:
	/** ctor. */
	StretchRect(int parts = 0, const TextureRef& texture = TextureRef());

	/** ctor, creating standard 9-slice stretching rect. */
	StretchRect(const TextureRef& texture, int left, int top, int right, int bottom, const RectF& texRect = RectF(),
				const Color4& color = Color4::White());

	/** copy-ctor. */
	StretchRect(const StretchRect& stretchRect);

	/** copy operator. */
	StretchRect& operator=(const StretchRect& stretchRect);

	/** Returns a copy of drawable. */
	IRectDrawable* Clone() const;

	/** Adding a part. Automatically resizing mesh. */
	int AddPart(const Vec2F& LTPercent, const Vec2F& LTPixel, const Vec2F& RBPercent, const Vec2F& RBPixel,
				const RectF& texRect, bool wrapTexture = false,
				const Color4& vertex0Color = Color4::White(), const Color4& vertex1Color = Color4::White(),
				const Color4& vertex2Color = Color4::White(), const Color4& vertex3Color = Color4::White());

	/** Removes part. */
	void RemovePart(int idx);

	/** Drawing. */
	void Draw();

	/** Drawing debug frames for each parts. */
	void DrawDebug();

	/** Sets the minimal size. */
	void SetMinSize(const Vec2F& minSize);

	/** Returns minimal size. */
	Vec2F GetMinSize() const;

protected:
	/** Calls when position changed. */
	void PositionChanged();

	/** Calls when size changed. */
	void SizeChanged();

	/** Calls when pivot changed. */
	void PivotChanged();

	/** Calls when color changed. */
	void ColorChanged();

	/** Creates mesh for some parts count. */
	void CreateMesh(int partsCount, const TextureRef& texture);

	/** Initialize mesh polygons for parts. */
	void InitMeshPolygons(int startIdx = 0);

	/** Updating mesh. */
	void UpdateMesh();

	/** Updating mesh colors. */
	void UpdateColors();
};

CLOSE_O2_NAMESPACE