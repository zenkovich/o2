#pragma once

#include "UI/Layer.h"
#include "UI/State.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Math/Layout.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Property.h"
#include "Utils/Tree.h"

namespace o2
{
	class Widget
	{
	public:
		typedef Vector<Ptr<Widget>> WidgetsVec;

	public:
		Property<Ptr<Widget>>                parent;
		Property<Vec2I>                      position;
		Property<Vec2I>                      absPosition;
		Property<Vec2I>                      size;
		Getter<WidgetsVec>                   childs;
		Getter<LayersVec>                    layers;
		Getter<StatesVec>                    states;
		Accessor<Ptr<Widget>, const String&> child;
		Accessor<Ptr<Layer>, const String&>  layer;
		Accessor<Ptr<State>, const String&>  state;


		Widget();

		Widget(const Widget& other);

		virtual ~Widget();

		Widget& operator=(const Widget& other);

		virtual Ptr<Widget> Clone() const;


		virtual void Update(float dt);

		virtual void Draw();


		virtual Ptr<Widget> GetParent() const;

		virtual void SetParent(Ptr<Widget> parent);

		virtual Ptr<Widget> AddChild(Ptr<Widget> widget);

		virtual bool RemoveChild(const String& path);

		virtual bool RemoveChild(Ptr<Widget> widget, bool release = true);

		virtual Ptr<Widget> GetChild(const String& path);

		virtual void RemoveAllChilds();

		virtual const WidgetsVec& GetChilds() const;


		virtual void SetPosition(const Vec2I& position);

		virtual Vec2I GetPosition() const;

		virtual void SetAbsolutePosition(const Vec2I& position);

		virtual Vec2I GetAbsolutePosition() const;

		virtual void SetSize(const Vec2I& size);

		virtual Vec2I GetSize() const;


		Ptr<Layer> AddLayer(Ptr<Layer> layer);

		Ptr<Layer> GetLayer(const String& path) const;

		bool RemoveLayer(Ptr<Layer> layer);

		bool RemoveLayer(const String& path);

		void RemoveAllLayers();

		const LayersVec& GetLayers() const;


		void SetState(const String& name, bool state);

		bool GetState(const String& name) const;

		Ptr<State> GetStateObject(const String& name) const;

		const StatesVec& GetStates() const;

	protected:
		String      mName;
		Vec2I       mPosition;
		Vec2I       mAbsolutePosition;
		Vec2I       mSize;

		LayersVec   mLayers;
		StatesVec   mStates;

		Ptr<Widget> mParent;
		WidgetsVec  mChilds;

		LayersVec   mDrawingLayers;

	protected:
		void UpdateLayout();
		void UpdateLayoutRecursive();

		void UpdateLayersDrawingSequence();

		WidgetsVec GetChildsNonConst();
		LayersVec GetlayersNonConst();
		StatesVec GetStatesNonConst();

		void InitializeProperties();

		friend class Layer;
	};
}