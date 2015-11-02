#pragma once

#include "Render/RectDrawable.h"
#include "Utils/Math/Layout.h"
#include "Utils/Memory/Ptr.h"
#include "Utils/Serialization.h"
#include "Utils/String.h"
#include "Utils/Tree.h"

namespace o2
{
	class Widget;

	class WidgetLayer;
	typedef Vector<Ptr<WidgetLayer>>  LayersVec;

	// ---------------------
	// Widget drawable layer
	// ---------------------
	class WidgetLayer: public ISerializable, public ITreeNode<WidgetLayer>
	{
	public:
		Layout             layout;   // Drawable layout
		String             name;     // Name of layer
		Property<float>    depth;    // Drawing depth (higher depths will draw later)
		Ptr<IRectDrawable> drawable; // Drawable

		// Default constructor
		WidgetLayer();

		// Copy-constructor
		WidgetLayer(const WidgetLayer& other);

		// Destructor
		~WidgetLayer();

		// Copy-operator
		WidgetLayer& operator=(const WidgetLayer& other);

		// Draws drawable
		void Draw();

		// Updates drawable and layout
		void Update(float dt);

		// Returns child layer by path ()
		Ptr<WidgetLayer> GetChild(const String& path);

		// Returns all child layers
		LayersVec GetAllChilds() const;

		// Sets depth
		void SetDepth(float depth);

		// Returns depth
		float GetDepth() const;

		SERIALIZABLE_IMPL(WidgetLayer);

		IOBJECT(WidgetLayer)
		{
			SRLZ_FIELD(layout);
			SRLZ_FIELD(name);
			SRLZ_FIELD(mDepth);
			SRLZ_FIELD(drawable);

			FIELD(mAbsolutePosition);
			//FIELD(mOwnerWidget);
		}

	protected:
		float       mDepth;            // Depth of drawable
		RectF       mAbsolutePosition; // Result absolute drawable position
		Ptr<Widget> mOwnerWidget;      // Owner widget pointer

		// Calls when added new child layer, sets his owner same as this owner and calls UpdateLayersDrawingSequence in owner
		void OnChildAdded(Ptr<WidgetLayer> child);

		// Updates drawable rect, calling when widget's layout was changed
		void UpdateLayout();

		// Initializes properties
		void InitializeProperties();

		friend class Widget;
	};
}
