#pragma once

#include "Application/Input.h"
#include "Utils/Delegates.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Property.h"

using namespace o2;

namespace o2
{
	class Sprite;
}

namespace Editor
{
	// -----------------
	// Scene drag handle
	// -----------------
	class SceneDragHandle
	{
	public:
		Sprite*                      regularSprite;       // Regular view sprite
		Sprite*                      hoverSprite;         // Hovered view sprite
		Sprite*                      pressedSprite;       // Pressed view sprite
		Function<void(const Vec2F&)> onChangedPos;        // On position changed event (parameter - position in scene)
		Function<void(const Vec2F&)> onMoved;             // On moved event (parameter - delta in scene)
		Function<void()>             onPressed;           // On cursor pressed event
		Function<void()>             onReleased;          // On cursor released event
		Property<Vec2F>              position;            // Current position in scene property
		Property<float>              angle;               // Handle angle in radians property
		Property<bool>               enabled;             // Handle enabling property
		CursorType                   cursorType;          // Cursor type when hovering and dragging
		bool                         pixelPerfect = true; // Is handle draws pixel perfect

		// Default constructor
		SceneDragHandle();

		// Constructor with views
		SceneDragHandle(Sprite* regular, Sprite* hover = nullptr, Sprite* pressed = nullptr);

		// Copy-constructor
		SceneDragHandle(const SceneDragHandle& other);

		// Destructor
		~SceneDragHandle();

		// Copy-operator
		SceneDragHandle& operator=(const SceneDragHandle& other);

		// Draws handle
		void Draw();

		// Returns true if point in screen space is above this
		bool IsUnderPoint(const Vec2F& point);

		// Sets position in scene
		void SetPosition(const Vec2F& position);

		// Returns position in scene
		Vec2F GetPosition() const;

		// Sets handle rotation angle in radiand
		void SetAngle(float rad);

		// Return handle rotation angle in radians
		float GetAngle() const;

		// Sets enabled
		void SetEnabled(bool enable);

		// Returns is handle enabled
		bool IsEnabled() const;

		// Returns is handle pressed
		bool IsPressed() const;

	protected:
		Vec2F mPosition;          // Current handle position
		Vec2F mPressOffset;       // Offset from cursor to position when pressing
		bool  mIsPressed = false; // Is handle pressed
		bool  mEnabled = true;    // Is handle enabled
		float mAngle = 0.0f;      // Handle rotation angle in radians

	protected:
		// Calls when cursor pressed on this
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time)
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor pressing was broken (when scrolled scroll area or some other)
		void OnCursorPressBreak(const Input::Cursor& cursor);

		// Calls when cursor stay down during frame
		void OnCursorStillDown(const Input::Cursor& cursor);

		// Calls when cursor enters this object
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object
		void OnCursorExit(const Input::Cursor& cursor);

		// Initializes properties
		void InitializeProperties();

		friend class SceneEditScreen;
	};
}