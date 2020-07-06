#pragma once
#include "o2/Utils/Math/Basis.h"
#include "o2/Utils/Types/Containers/Vector.h"
#include "o2/Events/CursorAreaEventsListener.h"

namespace o2
{
	class CursorAreaEventListenersLayer: public CursorAreaEventsListener
	{
	public:
		Basis drawnTransform;
		Basis viewPortBasis;
		Basis renderBasis;

		Vector<CursorAreaEventsListener*> cursorEventAreaListeners;

	public:
		void OnBeginDraw();
		void OnEndDraw();

		void OnDrawn(const Basis& transform);

		Vec2F ToLocal(const Vec2F& point);
		Vec2F FromLocal(const Vec2F& point);

	private:
		Basis mLocalToWorldTransform;
	};
}
