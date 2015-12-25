#pragma once

#include "ui_widget.h"
#include "util/graphics/rect_drawable.h"

OPEN_O2_NAMESPACE

/** Widget with drawables tree. */
class UIDrawablesListWidget: public UIWidget
{
public: 
	/** Base drawable class. COntains name, drawable, parent and childs, layout. */
	class Drawable: public PropertyList
	{
		friend class UIDrawablesListWidget;
	public:
		typedef Array< Drawable*> DrawablesArr;

	protected:
		String         mName;           /** Name of drawable. */
		IRectDrawable* mDrawable;       /** Rect drawable. Null, if no drawable. */
		Layout         mLayout;         /** Layout. */
		Drawable*      mParentDrawable; /** Parent drawable. NULL if no parent. */
		DrawablesArr   mChildDrawables; /** Child drawables. */

	public:
		PROPERTY(Drawable, Layout) layout; /** Layout property. Using set/getlayout. */

		/** ctor. */
		Drawable() {}

		/** ctor. */
		Drawable(const String& name, IRectDrawable* drawable, const Layout& layout = Layout::Both(),
			     Drawable* parentDrawable = NULL);

		/** copy-ctor. */
		Drawable(const Drawable& drawable);

		/** dtor. */
		~Drawable();

		/** Copy operator. */
		Drawable& operator=(const Drawable& drw);

		/** Return name of drawable. */
		String GetName() const;

		/** Returns path id. */
		String GetPathId() const;

		/** Returns rect drawable. */
		IRectDrawable* GetDrawable();

		/** Sets drawable. */
		void SetDrawable(IRectDrawable* drawable);

		/** Adding child drawable. */
		Drawable* AddChildDrawable(Drawable* drawable);

		/** Adding child drawable. */
		Drawable* AddChildDrawable(const String& name, IRectDrawable* drawable, 
			                       const Layout& layout = Layout::Both());

		/** Returns child drawable by path. */
		Drawable* GetChildDrawable(const String& path);

		/** Removes child drawable. */
		void RemoveChildDrawable(Drawable* drawable, bool release = true);

		/** Removes all drawables. */
		void RemoveAllChildDrawables();

		/** Sets layout. */
		void SetLayout(const Layout& layout);

		/** Returns layout. */
		Layout GetLayout() const;

		/** Updates layout. */
		void UpdateLayoutManual(const Vec2F& parPos, const Vec2F& parSize);

		/** Updates layout by parent. */
		void UpdateLayout();

		/** Drawing. */
		void Draw();

	protected:
		/** Initializing properties. */
		void InitializeProperties();

		/** Calls when layout was updated. */
		void LayoutUpdated();
	};
	typedef Array<Drawable*> DrawablesArr;

protected:
	Drawable mBasicDrawable; /** Basic drawable. */

public:
	/** ctor. */
	UIDrawablesListWidget(const Layout& layout, const String& id = "");

	/** copy-ctor. */
	UIDrawablesListWidget(const UIDrawablesListWidget& widget);

	/** dtor. */
	~UIDrawablesListWidget();

	/** Returns clone of widget. */
	virtual UIWidget* Clone() const;

	/** Adding drawable. */
	Drawable* AddDrawable(Drawable* drawable);

	/** Adding drawable with specified id. */
	Drawable* AddDrawable(IRectDrawable* drawable, const String& id, const Layout& layout = Layout::Both());

	/** Returns drawable by id. */
	Drawable* GetDrawable(const String& path);

	/** Removes drawable. */
	void RemoveDrawable(const String& id, bool release = true);

	/** Removes all drawables. */
	void RemoveAllDrawables();

protected:
	/** Drawing current widget. */
	virtual void LocalDraw();

	/** Calls when widget's layout updated. */
	virtual void LayoutUpdated();

	/** Calls when added drawable. */
	virtual void AddedDrawable(Drawable* drawable) {}
};

CLOSE_O2_NAMESPACE
