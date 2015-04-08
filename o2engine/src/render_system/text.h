#pragma once

#include "public.h"
#include "font.h"
#include "util/graphics/rect_drawable.h"
#include "util/property.h"
#include "util/string.h"

OPEN_O2_NAMESPACE

class Mesh;
class RenderSystem;

/** Text renderer class. Using font, basis and many style parameters. Caching text into meshes. */
class Text: public IRectDrawable
{
public:
	enum { nMeshMaxPolyCount = 4096 };

protected:
	typedef Array< Mesh* > MeshesArr;

	WString        mText;               /** Wide char string, containing rendering text. */
	Font*          mFont;               /** Using font. */
	Basis          mTransform;          /** Transformation. */
	BasisDef       mTransformDef;       /** Transform definition. */
	float          mCharactersDistCoef; /** Characters distance coef, 1 is standard. */
	float          mLinesDistCoef;      /** Lines distance coef, 1 is standard. */
	Font::VerAlign mVerAlign;           /** Vertical align. */
	Font::HorAlign mHorAlign;           /** Horizontal align. */
	bool           mWordWrap;           /** True, when words wrapping. */

	MeshesArr      mMeshes;             /** Meshes vector. */
	Basis          mLastTransform;      /** Last mesh update transformation. */
	bool           mNeedUpdateMesh;     /** True, when need rebuild meshes. */
	bool           mNeedTransformMesh;  /** True, when need transform meshes. */

	Font::TextSymbolsSet mSymbolsSet;   /** Symbols set definition. */

public:
	//properties
	PROPERTY(Text, Font*)          font;                /** Font pointer property. Uses set/getFont. */
	PROPERTY(Text, WString)        text;                /** Text property, wstring. Uses set/getText. */
	PROPERTY(Text, String)         ctext;               /** Text property, string. Uses set/getCText. */
	PROPERTY(Text, Font::VerAlign) verAlign;            /** vertical align property. Uses set/getVerAlign. */
	PROPERTY(Text, Font::HorAlign) horAlign;            /** Horizontal align property. Uses set/getHorAlign. */
	PROPERTY(Text, bool)           wordWrap;            /** Words wrapping flag property. Uses set/getWordWrap. */
	PROPERTY(Text, float)          angle;               /** Angle of rotation property. Uses set/getAngle. */
	PROPERTY(Text, Vec2F)          scale;               /** Scale property. Uses set/getScale. */
	PROPERTY(Text, float)          charactersHeight;    /** Characters height property, pixels. Uses set/getCharactersHeight. */
	PROPERTY(Text, Basis)          transform;           /** Transformation property. Uses set/getTransform. */
	PROPERTY(Text, BasisDef)       transformDef;        /** Transform definition property. Uses set/getTransformDef. */
	PROPERTY(Text, float)          charactersDistCoef;  /** Characters distance coef, 1 is standard. Uses set/getCharactersDistCoef. */
	PROPERTY(Text, float)          linesDistCoef;       /** Lines distance coef, 1 is standard. Uses set/getLinesDistCoef. */

	/** ctor. */
	Text(Font* font);

	/** ctor. */
	Text(const Text& text);

	/** dtor. */
	~Text();

	/** Returns copy of drawable. */
	IRectDrawable* Clone() const;

	/** Draw text. */
	void Draw();

	/** Sets using font. */
	void SetFont(Font* font);

	/** Returns using font. */
	Font* GetFont() const;

	/** Sets text. */
	void GetText(const WString& text);

	/** Returns text. */
	WString GetText() const;

	/** Sets text. */
	void SetCText(const String& text);

	/** Returns text as string. */
	String GetCText() const;

	/** Sets rotation angle. */
	void SetAngle(const float& angle);

	/** Returns rotation angle. */
	float GetAngle() const;

	/** Sets text scale. */
	void SetScale(const Vec2F& scale);

	/** Returns text scale. */
	Vec2F GetScale() const;

	/** Sets characters height in pixels. */
	void SetCharactersHeight(const float& height);

	/** Returns characters height in pixels. */
	float GetCharactersHeight() const;

	/** Sets transformation of text. */
	void SetTransform(const Basis& bas);

	/** Returns text transformation. */
	Basis GetTransform() const;

	/** Sets text transformation definition. */
	void SetTransformDef(const BasisDef& def);

	/** Returns text transformation definition. */
	BasisDef GetTransformDef() const;

	/** Sets horizontal align. */
	void SetHorAlign(const Font::HorAlign& align);

	/** Returns horizontal align. */
	Font::HorAlign GetHorAlign() const;

	/** Sets vertical align. */
	void SetVerAlign(const Font::VerAlign& align);

	/** returns vertical align. */
	Font::VerAlign GetVerAlign() const;

	/** Sets word wrapping. */
	void SetWordWrap(const bool& flag);

	/** Returns word wrapping. */
	bool GetWordWrap() const;

	/** Sets characters distance coefficient. */
	void SetCharactersDistCoef(const float& coef = 1);

	/** Returns characters distance coef. */
	float GetCharactersDistCoef() const;

	/** Sets lines distance coefficient. */
	void SetLinesDistCoef(const float& coef = 1);

	/** Returns lines distance coefficient. */
	float GetLinesDistCoef() const;

	/** Returns symbol set structure pointer. */
	Font::TextSymbolsSet* GetSymbolsSet();

	/** Returns real text size. */
	Vec2F GetRealSize();

	/** Updates mesh immediately, don't wait drawing. */
	void ForceUpdateMesh();

protected:
	/** Calls when position was changed. */
	void PositionChanged();

	/** Calls when size was changed. */
	void SizeChanged();

	/** Calls when pivot was changed. */
	void PivotChanged();

	/** Calls when color was changed. */
	void ColorChanged();

	/** Initializing properties. */
	void InitializeProperties();

	/** Updating meshes. */
	void UpdateMesh();

	/** Transforming meshes by basis. */
	void TransformMesh(const Basis& bas);

	/** Preparing meshes for characters count. */
	void PrepareMesh(int charactersCount);
};

CLOSE_O2_NAMESPACE
