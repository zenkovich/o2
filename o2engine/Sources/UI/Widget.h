#pragma once

#include "Render/Text.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetState.h"
#include "Utils/Containers/Vector.h"
#include "Utils/Math/Layout.h"
#include "Utils/Math/Layout.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Property.h"
#include "Utils/Serialization.h"
#include "Utils/Tree.h"

namespace o2
{
	class ImageAsset;
	class VectorFontAsset;
	class BitmapFontAsset;

	class Widget: public ISerializable
	{
	public:
		typedef Vector<Ptr<Widget>> WidgetsVec;

	public:
		Property<Ptr<Widget>>                      parent;
		Property<Vec2I>                            position;
		Property<Vec2I>                            absPosition;
		Property<Vec2I>                            size;
		Getter<WidgetsVec>                         childs;
		Getter<LayersVec>                          layers;
		Getter<StatesVec>                          states;
		Accessor<Ptr<Widget>, const String&>       child;
		Accessor<Ptr<WidgetLayer>, const String&>  layer;
		Accessor<Ptr<WidgetState>, const String&>  state;


		Widget();

		Widget(const Widget& other);

		virtual ~Widget();

		Widget& operator=(const Widget& other);


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


		Ptr<WidgetLayer> AddLayer(Ptr<WidgetLayer> layer);

		Ptr<WidgetLayer> AddLayer(const String& name, Ptr<IRectDrawable> drawable,
								  const Layout& layout = Layout::Both(), float depth = 0.0f);

		Ptr<WidgetLayer> AddSpriteLayer(const String& name, const String& fileName,
										const Layout& layout = Layout::Both(), float depth = 0.0f);

		Ptr<WidgetLayer> AddSpriteLayer(const String& name, AssetId assetId,
										const Layout& layout = Layout::Both(), float depth = 0.0f);

		Ptr<WidgetLayer> AddSpriteLayer(const String& name, Ptr<ImageAsset> asset,
										const Layout& layout = Layout::Both(), float depth = 0.0f);

		Ptr<WidgetLayer> AddTextLayer(const String& name, const String& text, const String& fontFileName,
									  Text::HorAlign horAlign = Text::HorAlign::Middle,
									  Text::VerAlign verAlign = Text::VerAlign::Middle,
									  const Layout& layout = Layout::Both(), float depth = 0.0f);

		Ptr<WidgetLayer> AddTextLayer(const String& name, const String& text, AssetId fontAssetId,
									  Text::HorAlign horAlign = Text::HorAlign::Middle,
									  Text::VerAlign verAlign = Text::VerAlign::Middle,
									  const Layout& layout = Layout::Both(), float depth = 0.0f);

		Ptr<WidgetLayer> AddTextLayer(const String& name, const String& text, Ptr<VectorFontAsset> fontAsset,
									  Text::HorAlign horAlign = Text::HorAlign::Middle,
									  Text::VerAlign verAlign = Text::VerAlign::Middle,
									  const Layout& layout = Layout::Both(), float depth = 0.0f);

		Ptr<WidgetLayer> AddTextLayer(const String& name, const String& text, Ptr<BitmapFontAsset> fontAsset,
									  Text::HorAlign horAlign = Text::HorAlign::Middle,
									  Text::VerAlign verAlign = Text::VerAlign::Middle,
									  const Layout& layout = Layout::Both(), float depth = 0.0f);

		Ptr<WidgetLayer> GetLayer(const String& path) const;

		bool RemoveLayer(Ptr<WidgetLayer> layer);

		bool RemoveLayer(const String& path);

		void RemoveAllLayers();

		const LayersVec& GetLayers() const;

		Ptr<WidgetState> AddState(const String& name);

		Ptr<WidgetState> AddState(const String& name, const Animation& animation);

		Ptr<WidgetState> AddState(Ptr<WidgetState> state);

		bool RemoveState(const String& name);

		bool RemoveState(Ptr<WidgetState> state);

		void RemoveAllStates();

		void SetState(const String& name, bool state);

		bool GetState(const String& name) const;

		Ptr<WidgetState> GetStateObject(const String& name) const;

		const StatesVec& GetStates() const;

		SERIALIZABLE_IMPL(Widget);

		IOBJECT(Widget)
		{
			FIELD(parent);
			FIELD(position);
			FIELD(absPosition);
			FIELD(size);
			FIELD(childs);
			FIELD(layers);
			FIELD(states);
			FIELD(child);
			FIELD(layer);
			FIELD(state);

			SRLZ_FIELD(mName);
			SRLZ_FIELD(mPosition);
			SRLZ_FIELD(mSize);
			SRLZ_FIELD(mLayers);
			SRLZ_FIELD(mStates);
			SRLZ_FIELD(mChilds);

			FIELD(mDrawingLayers);
			FIELD(mParent);
			FIELD(mAbsolutePosition);
		}

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

		Dictionary<String, Ptr<WidgetLayer>> GetAllLayers();

		virtual void OnLayerAdded(Ptr<WidgetLayer> layer);

		void InitializeProperties();

		friend class WidgetLayer;
	};
}