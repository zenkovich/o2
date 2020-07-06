#include "o2/stdafx.h"
#include "CursorAreaEventsListenersLayer.h"

#include "o2/Events/EventSystem.h"

namespace o2
{
	void CursorAreaEventListenersLayer::OnBeginDraw()
	{
		viewPortBasis = o2Render.GetCamera().GetBasis();
		o2Events.SetCursorAreaEventsListenersLayer(this);
	}

	void CursorAreaEventListenersLayer::OnEndDraw()
	{
		renderBasis = Camera().GetBasis();
		o2Events.SetCursorAreaEventsListenersLayer(nullptr);
	}

	void CursorAreaEventListenersLayer::OnDrawn(const Basis& transform)
	{
		drawnTransform = transform;

		mLocalToWorldTransform = viewPortBasis.Inverted()*renderBasis*renderBasis.Inverted()*drawnTransform;

		CursorAreaEventsListener::OnDrawn();
	}

	Vec2F CursorAreaEventListenersLayer::ToLocal(const Vec2F& point)
	{
		return point*mLocalToWorldTransform.Inverted();
	}

	Vec2F CursorAreaEventListenersLayer::FromLocal(const Vec2F& point)
	{
		return point*mLocalToWorldTransform;
	}

}