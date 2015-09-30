#pragma once

#include "Utils/Tree.h"
#include "Utils/Math/Layout.h"
#include "Utils/String.h"
#include "Utils/Memory/Ptr.h"

namespace o2
{
	class Widget;
	class IRectDrawable;

	class Layer;
	typedef Vector<Ptr<Layer>>  LayersVec;

	class Layer: public ITreeNode<Layer>
	{
	public:
		Layout layout;
		String id;

		Layer();
		~Layer();

		void Draw();
		void Update(float dt);

		Ptr<Layer> GetChild(const String& path);

		LayersVec GetAllChilds() const;

		void SetDepth(float depth);
		float GetDepth() const;

	protected:
		float              mDepth;
		RectF              mAbsolutePosition;
		Ptr<Widget>        mOwnerWidget;
		Ptr<IRectDrawable> mDrawable;

		void OnChildAdded(Ptr<Layer> child);

		friend class Widget;
	};
}
