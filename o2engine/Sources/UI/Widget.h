#pragma once

#include "Render/Text.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
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

	// ------------------------------------------------------
	// Basic UI Widget. Its a simple and basic element of UI, 
	// everything other UI's are based on this
	// ------------------------------------------------------
	class Widget: public ISerializable
	{
	public:
		typedef Vector<Ptr<Widget>> WidgetsVec;

	public:
		Property<String>                          name;
		Property<Ptr<Widget>>                     parent;
		Getter<WidgetsVec>                        childs;
		Getter<LayersVec>                         layers;
		Getter<StatesVec>                         states;
		Accessor<Ptr<Widget>, const String&>      child;
		Accessor<Ptr<WidgetLayer>, const String&> layer;
		Accessor<Ptr<WidgetState>, const String&> state;
		WidgetLayout                              layout;

		// Default constructor
		Widget();

		// Copy-constructor
		Widget(const Widget& other);

		// Virtual destructor
		virtual ~Widget();

		// Copy-operator
		Widget& operator=(const Widget& other);

		// Updates drawables, states and widget
		virtual void Update(float dt);

		// Draws widget
		virtual void Draw();

		// Sets widget name
		virtual void SetName(const String& name);

		// Returns widget name
		virtual String GetName() const;

		// Returns parent widget
		virtual Ptr<Widget> GetParent() const;

		// Sets widget parent
		virtual void SetParent(Ptr<Widget> parent);

		// Adds child widget
		virtual Ptr<Widget> AddChild(Ptr<Widget> widget);

		// Removes child by path
		virtual bool RemoveChild(const String& path);

		// Removes child widget
		virtual bool RemoveChild(Ptr<Widget> widget, bool release = true);

		// Returns child by path. Returns nullptr if isn't exist
		virtual Ptr<Widget> GetChild(const String& path);

		// Removes all child widgets
		virtual void RemoveAllChilds();

		// Returns constant children widgets vector
		virtual const WidgetsVec& GetChilds() const;

		// Adds layer
		Ptr<WidgetLayer> AddLayer(Ptr<WidgetLayer> layer);

		// Adds layer
		Ptr<WidgetLayer> AddLayer(const String& name, Ptr<IRectDrawable> drawable,
								  const Layout& layout = Layout::Both(), float depth = 0.0f);

		// Adds sprite layer
		Ptr<WidgetLayer> AddSpriteLayer(const String& name, const String& fileName,
										const Layout& layout = Layout::Both(), float depth = 0.0f);

		// Adds sprite layer
		Ptr<WidgetLayer> AddSpriteLayer(const String& name, AssetId assetId,
										const Layout& layout = Layout::Both(), float depth = 0.0f);

		// Adds sprite layer
		Ptr<WidgetLayer> AddSpriteLayer(const String& name, Ptr<ImageAsset> asset,
										const Layout& layout = Layout::Both(), float depth = 0.0f);

		// Adds text layer
		Ptr<WidgetLayer> AddTextLayer(const String& name, const String& text, const String& fontFileName,
									  Text::HorAlign horAlign = Text::HorAlign::Middle,
									  Text::VerAlign verAlign = Text::VerAlign::Middle,
									  const Layout& layout = Layout::Both(), float depth = 0.0f);

		// Adds text layer
		Ptr<WidgetLayer> AddTextLayer(const String& name, const String& text, AssetId fontAssetId,
									  Text::HorAlign horAlign = Text::HorAlign::Middle,
									  Text::VerAlign verAlign = Text::VerAlign::Middle,
									  const Layout& layout = Layout::Both(), float depth = 0.0f);

		// Adds text layer
		Ptr<WidgetLayer> AddTextLayer(const String& name, const String& text, Ptr<VectorFontAsset> fontAsset,
									  Text::HorAlign horAlign = Text::HorAlign::Middle,
									  Text::VerAlign verAlign = Text::VerAlign::Middle,
									  const Layout& layout = Layout::Both(), float depth = 0.0f);

		// Adds text layer
		Ptr<WidgetLayer> AddTextLayer(const String& name, const String& text, Ptr<BitmapFontAsset> fontAsset,
									  Text::HorAlign horAlign = Text::HorAlign::Middle,
									  Text::VerAlign verAlign = Text::VerAlign::Middle,
									  const Layout& layout = Layout::Both(), float depth = 0.0f);

		// Returns layer by path. Returns null if layer isn't exist
		Ptr<WidgetLayer> GetLayer(const String& path) const;

		// Returns layer by path. Returns null if layer isn't exist or layer drawable has different type
		template<typename _type>
		Ptr<_type> GetLayerDrawable(const String& path) const;

		// Removes layer
		bool RemoveLayer(Ptr<WidgetLayer> layer);

		// Removes layer
		bool RemoveLayer(const String& path);

		// Removes all layers
		void RemoveAllLayers();

		// Returns all layers
		const LayersVec& GetLayers() const;

		// Adds new state with name
		Ptr<WidgetState> AddState(const String& name);

		// Adds new state with name and animation
		Ptr<WidgetState> AddState(const String& name, const Animation& animation);

		// Adds state
		Ptr<WidgetState> AddState(Ptr<WidgetState> state);

		// Removes state by name
		bool RemoveState(const String& name);

		// Removes state
		bool RemoveState(Ptr<WidgetState> state);

		// Removes all states
		void RemoveAllStates();

		// Sets state value
		void SetState(const String& name, bool state);

		// Returns state value
		bool GetState(const String& name) const;

		// Returns state object by name
		Ptr<WidgetState> GetStateObject(const String& name) const;

		// Returns all states
		const StatesVec& GetStates() const;

		SERIALIZABLE_IMPL(Widget);

		IOBJECT(Widget)
		{
			FIELD(name);
			FIELD(parent);
			FIELD(layout);
			FIELD(childs);
			FIELD(layers);
			FIELD(states);
			FIELD(child);
			FIELD(layer);
			FIELD(state);

			SRLZ_FIELD(mName);
			SRLZ_FIELD(layout);
			SRLZ_FIELD(mLayers);
			SRLZ_FIELD(mStates);
			SRLZ_FIELD(mChilds);

			FIELD(mDrawingLayers);
			FIELD(mParent);
		}

	protected:
		String      mName;          // Name
								    
		LayersVec   mLayers;        // Layers array
		StatesVec   mStates;        // States array
								    
		Ptr<Widget> mParent;        // Parent widget
		WidgetsVec  mChilds;        // Children widgets

		LayersVec   mDrawingLayers; // Layers ordered by depth

	protected:
		// Updates children layouts
		void UpdateLayoutRecursive();

		// Calls when layout was updated
		virtual void OnLayoutUpdated();

		// Updates layers layouts, calls after updating widget layout
		void UpdateLayersLayouts();

		// Updates layers drawing sequence
		void UpdateLayersDrawingSequence();

		// Returns children widgets (for property)
		WidgetsVec GetChildsNonConst();

		// Returns layers (for property)
		LayersVec GetLayersNonConst();

		// Returns states (for property)
		StatesVec GetStatesNonConst();

		// Returns dictionary of all layers by names
		Dictionary<String, Ptr<WidgetLayer>> GetAllLayers();

		// Calls when layer added and updates drawing sequence
		virtual void OnLayerAdded(Ptr<WidgetLayer> layer);

		// Calls when child widget was added
		virtual void OnChildAdded(Ptr<Widget> child);

		// Calls when child widget was removed
		virtual void OnChildRemoved(Ptr<Widget> child);

		// Calls when deserialized
		void OnDeserialized(const DataNode& node);

		// Initializes properties
		void InitializeProperties();

		friend class WidgetLayer;
		friend class WidgetLayout;
	};

	template<typename _type>
	Ptr<_type> Widget::GetLayerDrawable(const String& path) const
	{
		auto layer = GetLayer(path);
		if (layer && layer->drawable->GetTypeId() == _type::type->ID())
			return layer->drawable.Cast<_type>();

		return nullptr;
	}

}