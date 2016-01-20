#pragma once

#include "Events/CursorEventsListener.h"
#include "Utils/Delegates.h"
#include "Utils/Property.h"

namespace o2
{
	class Sprite;

	class DragHandle: public CursorEventsListener
	{
	public:
		Sprite*                      regularSprite;
		Sprite*                      hoverSprite;
		Sprite*                      pressedSprite;
		Function<void(const Vec2F&)> onChangedPos;
		Property<Vec2F>              position;

		DragHandle();
		DragHandle(Sprite* regular, Sprite* hover = nullptr, Sprite* pressed = nullptr);
		DragHandle(const DragHandle& other);
		~DragHandle();

		DragHandle& operator=(const DragHandle& other);

		void Draw();

		bool IsUnderPoint(const Vec2F& point);
		float Depth();

		void OnCursorPressed(const Input::Cursor& cursor);
		void OnCursorReleased(const Input::Cursor& cursor);
		void OnCursorStillDown(const Input::Cursor& cursor);
		void OnCursorEnter(const Input::Cursor& cursor);
		void OnCursorExit(const Input::Cursor& cursor);

		void SetPosition(const Vec2F& position);
		Vec2F GetPosition() const;

	protected:
		Vec2F mPosition;

	protected:
		void InitializeProperties();
	};
}
