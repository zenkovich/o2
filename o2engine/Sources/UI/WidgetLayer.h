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

	class WidgetLayer: public ITreeNode<WidgetLayer>, public ISerializable
	{
	public:
		Layout             layout;
		String             id;
		Property<float>    depth;
		Ptr<IRectDrawable> drawable;

		WidgetLayer();
		WidgetLayer(const WidgetLayer& other);
		~WidgetLayer();

		WidgetLayer& operator=(const WidgetLayer& other);

		void Draw();
		void Update(float dt);

		Ptr<WidgetLayer> GetChild(const String& path);

		LayersVec GetAllChilds() const;

		void SetDepth(float depth);
		float GetDepth() const;

		SERIALIZABLE_IMPL(WidgetLayer);

		IOBJECT(WidgetLayer)
		{
			SRLZ_FIELD(layout);
			SRLZ_FIELD(id);
			SRLZ_FIELD(mDepth);
			SRLZ_FIELD(drawable);

			FIELD(mAbsolutePosition);
			//FIELD(mOwnerWidget);
		}

	protected:
		float              mDepth;
		RectF              mAbsolutePosition;
		Ptr<Widget>        mOwnerWidget;

		void OnChildAdded(Ptr<WidgetLayer> child);

		void InitializeProperties();

		friend class Widget;
	};
}
