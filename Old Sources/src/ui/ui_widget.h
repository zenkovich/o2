#pragma once

#include <list>
#include <map>

#include "public.h"

#include "ui_state.h"
#include "util/callback.h"
#include "util/geometry/geometry.h"
#include "util/input/input_message.h"
#include "util/math/layout.h"
#include "util/math/vector2.h"
#include "util/property.h"
#include "util/property_list.h"
#include "util/type_indexation.h"

OPEN_O2_NAMESPACE

class Geometry;
class UIState;

/** Basic widget object. Contains id, parent, childes, position and other strange data. */
class UIWidget: public PropertyList
{
	friend class UIController;
	friend class UIState;
	friend class UISkinManager;
	friend class UIScrollArea;
	friend class UIStdSkinInitializer;
	friend class UIHorLayout;
	friend class UIVerLayout;

public:
	typedef Vector< UIWidget* > WidgetsVec;
	typedef std::map< String, UIState* > StatesMap;
	
protected:
	String         mId;                      /**< Identificator or name. */
	String         mHint;                    /**< Hint text. */
	UIWidget*      mParent;                  /**< Parent widget. NULL if no parent. */
	Layout         mLayout;                  /**< Widget layout. */
	Layout         mChildsLayout;            /**< Childes layout. */
	WidgetsVec     mChildWidgets;            /**< Childes widgets. */
	Vec2F          mGlobalPosition;          /**< Position in screen space. */
	Vec2F          mSize;                    /**< Size of widget. Not including childes. */
	Vec2F          mAbsMinSize;              /**< Absolute widget minimal size, depends on mLayout and child mResMinSize. */
	Vec2F          mFixedMinSize;            /**< Widgets fixed min size. */
	Vec2F          mChildsOffset;            /**< Offset for childrens. */
	Geometry*      mGeometry;                /**< Colliding geometry. May be NULL. */
	RectF          mBounds;                  /**< Widget with childes bounds. */
	bool           mResizeByChilds;          /**< Resize by childes, if true, widget will be resized so that all fit. */
	bool           mVisible;                 /**< True, if widget is visible. */
	bool           mFocused;                 /**< True, if widget on focus. */
	bool           mCursorInside;            /**< True, when cursor is inside widget. */
	float          mBasicTransparency;       /**< Basic transparency of widget. */
	float          mResTransparency;         /**< Result transparency of widget. */
				    				            
	StatesMap      mStates;                  /**< States map. */
	UIState*       mVisibleState;            /**< Shared to visible state. */
				    					     
	uint           mUpdatedAtFrame;          /** Last update frame index. */
	uint           mDrawedAtFrame;           /** Last drawing frame index. */
	uint           mProcessedInputAtFrame;   /** Last input processing frame index. */

	CallbackChain  mResTransparencyChanged;  /** Calls when result transparency changed. */
	ICallback*     mCheckParentTransparency; /** Calls when parent transparency changing. */

public:
	typedef UIWidget* uiWidgetPtr;

	//Type definition
	DEFINE_TYPE(UIWidget);

	CallbackChain onVisibleOn;
	CallbackChain onVisibleOff;

	//properties
	PROPERTY(UIWidget, UIWidget*) parent;         /**< Parent property. Using setParent/getParent. */
	PROPERTY(UIWidget, String)    id;             /**< Identificator property. Using setId/getId. */
	PROPERTY(UIWidget, Vec2F)     position;       /**< Local position property. Using setPosition/getPosition. */
	PROPERTY(UIWidget, Vec2F)     globalPosition; /**< Global position property. Using setGlobalPosition/get.. */
	PROPERTY(UIWidget, Vec2F)     size;           /**< Size property. Using setSize/getSize. */
	PROPERTY(UIWidget, bool)      visible;        /**< Visibility property. Using set/isVisible. */
	PROPERTY(UIWidget, Layout)    layout;         /**< Widget layout. Using set/getLayout. */
	PROPERTY(UIWidget, float)     transparency;   /**< Widget transparency. Using set/getTransparency. */


	/** ctor. */
	UIWidget(const Layout& layout, const String& id = "");

	/** copy-ctor. */
	UIWidget(const UIWidget& widget);

	/** dtor. */
	virtual ~UIWidget();

	/** Returns clone of widget. */
	virtual UIWidget* Clone() const;

	/** Draw widget and childes. */
	virtual void Draw();

	/** Update widget and childes. */
	virtual void Update(float dt);

	/** Process message in widget, and after in childes. */
	virtual bool ProcessInputMessage(const InputMessage& msg);

	/** Returns true, if point inside widget or inside childes. */
	virtual bool IsInside(const Vec2F& point) const;

	/** Adding child widget. If position negative, adding at end. */
	virtual UIWidget* AddChild(UIWidget* widget, int position = -1);

	/** Adding child widget and returns correct type. If position negative, adding at end. */
	template<typename T>
	T* AddTChild(T* widget, int position = -1)
	{
		return static_cast<T*>(AddChild(widget, position));
	}

	/** Removing child widget. */
	virtual void RemoveChild(UIWidget* widget, bool release = true);

	/** Remove all child widgets. */
	virtual void RemoveAllChilds();

	/** Get widget by id. Id format "some_child/child/", ".." - directs to parent. */
	template<typename T>
	T* GetWidgetByType(const String& id)
	{
		return (T*)(GetWidget(id));
	}

	/** Get widget by id. Id format "some_child/child/", ".." - directs to parent. */
	UIWidget* GetWidget(const String& id);

	/** Returns widget's path from this widget. */
	String GetWidgetPath(UIWidget* widget) const;


	//setters and getters
	/** Returns hint string. */
	virtual String GetHint() const { return ""; }

	/** Returns true, if widget can take focus. */
	virtual bool IsFocusable() const;

	/** Setting focus on widget. */
	void SetFocused(bool focused);

	/** Return true, when widget on focus. */
	bool IsFocused() const;

	/** Makes widget focused. */
	void MakeFocused();

	/** Releases widgets focus. */
	void ReleaseFocus();

	/** Adding new state to widget. */
	UIState* AddState(UIState* state);

	/** Setting state parameter. */
	void SetState(const String& stateId, bool value);

	/** Returns shared state by id. */
	UIState* GetState(const String& stateId);

	/** Sets widget visible. */
	void SetVisible(bool visible);

	/** Returns true, if widget is visible. */
	bool IsVisible() const;
	
	/** Sets widget's parent. */
	void SetParent(UIWidget* parent);

	/** Returns parent ptr. */
	UIWidget* GetParent();

	/** Sets the local position. */
	void SetPosition(const Vec2F& position);

	/** Returns local position. */
	Vec2F GetPosition() const;

	/** Sets the id. */
	void SetId(const String& id);

	/** Returns the id. */
	String GetId() const;

	/** Sets the global position. */
	void SetGlobalPosition(const Vec2F& position);

	/** Returns the global position. */
	Vec2F GetGlobalPosition() const;

	/** Sets the size. */
	void SetSize(const Vec2F& size);

	/** Returns size. */
	Vec2F GetSize() const;

	/** Sets widget layout. */
	void GetLayout(const Layout& layout);

	/** Returns widget layout. */
	Layout GetLayout() const;

	/** Sets childes layout. */
	void SetChildsLayout(const Layout& layout);

	/** Returns childes layout. */
	Layout GetChildsLayout() const;

	/** Sets the colliding geometry. */
	void SetGeometry(Geometry* geometry);

	/** Returns colliding geometry ptr. */
	Geometry* GetGeometry() const;

	/** Sets transparency of widget. */
	void SetTransparency(float transparency);

	/** Returns widget transparency. */
	float GetTransparency() const;

	/** Sets resizing by children. */
	void SetRisizeByChilds(bool flag);

	/** Returns true, if resizing by childs. */
	bool IsResizingByChilds() const;

protected:
	/** Updating current and child layouts: global positions and bounds. */
	virtual void UpdateLayout(bool force = false, bool onlyDownUpdates = false);

	/** Calls when added some state. */
	virtual void AddedState(UIState* state) {}

	/** Updating states. */
	virtual void UpdateStates(float dt);

	/** Drawing current widget. */
	virtual void LocalDraw() {}

	/** Updating current widget. */
	virtual void LocalUpdate(float dt) {}

	/** Updating current widget layout: global position and bounds. */
	virtual bool LocalUpdateLayout();

	/** Calls when widget's layout updated. */
	virtual void LayoutUpdated() {}

	/** Processing input message in current widget. */
	virtual bool LocalProcessInputMessage(const InputMessage& msg) { return false; }

	/** Returns true, if point inside current widget. */
	virtual bool IsLocalInside(const Vec2F& point) const { return true; }

	/** Calls when widget focused. */
	virtual void OnFocused();

	/** Calls when widget lost focus. */
	virtual void OnFocusLost();

	/** Calls when parent transparency changed. */
	virtual void UpdateResTransparency();

	/** Sets widget layout with parametres. */
	void SetLayoutParams(const Layout& layout, bool onlyDownUpdate);

	/** Calls when need to update result min size. */
	void UpdateAbsMinSize();

	/** Checking size by childs. */
	void CheckResizingByChilds();

	/** Initialize all properties. */
	void InitializeProperties();

	/** Setting mVisible value. */
	void SetVisibleParam(bool param);

	/** Drawing debug frame. */
	void DrawDebugFrame();
};

CLOSE_O2_NAMESPACE