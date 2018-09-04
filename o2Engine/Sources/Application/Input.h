#pragma once

#include "Utils/Types/CommonTypes.h"
#include "Utils/Types/Containers/Vector.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Property.h"
#include "Utils/Singleton.h"

#ifdef PLATFORM_ANDROID
#include "Application/Android/VKCodes.h"
#endif

// Input access macros
#define o2Input Input::Instance()

namespace o2
{
	// -------------------------------------------------------------------------
	// Input message. Containing pressed, down, released keys, cursors positions
	// -------------------------------------------------------------------------
	class Input: public Singleton<Input>
	{
	public:
		struct Cursor;
		struct Key;

		typedef Vector<Cursor> CursorsVec;
		typedef Vector<Key> KeysVec;

	public:
		PROPERTIES(Input);
		GETTER(bool, cursorPressed, IsCursorPressed);            // Is cursor pressed at current frame getter
		GETTER(bool, cursorDown, IsCursorDown);                  // Is cursor down at current frame getter
		GETTER(bool, cursorReleased, IsCursorReleased);          // Is cursor released at current frame getter
		GETTER(bool, altCursorPressed, IsRightMousePressed);     // Is alt cursor pressed at current frame getter (right mouse button)
		GETTER(bool, altCursorDown, IsRightMouseDown);           // Is alt cursor down at current frame getter (right mouse button)
		GETTER(bool, altCursorReleased, IsRightMouseDown);       // Is alt cursor released at current frame getter (right mouse button)
		GETTER(bool, alt2CursorPressed, IsMiddleMousePressed);   // Is alt2 cursor pressed at current frame getter (middle mouse button)
		GETTER(bool, alt2CursorDown, IsMiddleMouseDown);	     // Is alt2 cursor down at current frame getter (middle mouse button)
		GETTER(bool, alt2CursorReleased, IsMiddleMouseReleased); // Is alt2 cursor released at current frame getter (middle mouse button)
		GETTER(Vec2F, cursorPos, GetCursorPos);                  // Cursor position getter
		GETTER(Vec2F, cursorDelta, GetCursorDelta);              // Cursor position delta getter

		// Default constructor
		Input();

		// Returns true if key was pressed at current frame
		bool IsKeyPressed(KeyboardKey key) const;

		// Returns true if key was pressed some frames ago
		bool IsKeyDown(KeyboardKey key) const;

		// Returns true, if key was released at current frame
		bool IsKeyReleased(KeyboardKey key) const;

		// Returns true, if key was pressed and repeated after delay
		bool IsKeyRepeating(KeyboardKey key, float delay = 0.04f, float beginDelay = 0.5f) const;

		// Returns key pressing time.Returns  0, if key not pressed
		float GetKeyPressingTime(KeyboardKey key) const;

		// Returns position of cursor
		Vec2F GetCursorPos(CursorId id = 0) const;

		// Returns true, when cursor pressed at current frame
		bool IsCursorPressed(CursorId id = 0) const;

		// Returns true, when cursor is down
		bool IsCursorDown(CursorId id = 0) const;

		// Returns true, when cursor released at current frame
		bool IsCursorReleased(CursorId id = 0) const;

		// Returns cursor pressed time. 0 if cursor not pressed
		float GetCursorPressingTime(CursorId id = 0) const;

		// Returns cursor delta
		Vec2F GetCursorDelta(CursorId id = 0) const;

		// Returns true, when right mouse button pressed (right mouse button)
		bool IsRightMousePressed() const;

		// Returns true, when right mouse button down (right mouse button)
		bool IsRightMouseDown() const;

		// Returns true, when right mouse button released (right mouse button)
		bool IsRightMouseReleased() const;

		// Returns right mouse button pressed time (right mouse button). 0 if cursor not pressed
		float GetRightMousePressedTime() const;

		// Returns true, when middle mouse button pressed (medium mouse button)
		bool IsMiddleMousePressed() const;

		// Returns true, when middle mouse button down (medium mouse button)
		bool IsMiddleMouseDown() const;

		// Returns true, when middle mouse button released (medium mouse button)
		bool IsMiddleMouseReleased() const;

		// Returns middle mouse button pressed time (medium mouse button). 0 if cursor not pressed
		float GetMiddleMousePressedTime() const;

		// Returns mouse wheel delta at frame
		float GetMouseWheelDelta() const;

		// Returns cursor by id. Returns nullptr if no cursor with specified id
		Cursor* GetCursor(CursorId id = 0);

		// Returns cursors vector
		const CursorsVec& GetCursors() const;

		// Returns released cursors vector
		const CursorsVec& GetReleasedCursors() const;

		// Returns pressed keys
		KeysVec const& GetPressedKeys() const;

		// Returns pressed keys
		KeysVec const& GetDownKeys() const;

		// Returns pressed keys
		KeysVec const& GetReleasedKeys() const;

		// Call it after frame update
		void Update(float dt);

	public:
		// -----------------
		// Cursor definition
		// -----------------
		struct Cursor
		{
			Vec2F    position;    // Current cursor position, in pixels, from left top corner 
			Vec2F    delta;       // Cursor moving delta between frames
			CursorId id;          // Cursor id
			float    pressedTime; // Time until key is pressed in seconds
			bool     isPressed;   // True when cursor is pressed down

		public:
			// Constructor
			Cursor(const Vec2F& position = Vec2F(), CursorId id = 0);

			// Equals operator
			bool operator==(const Cursor& other);
		};

		// ---------------------
		// Key with pressed time
		// ---------------------
		struct Key
		{
			KeyboardKey keyCode;     // Key id
			float       pressedTime; // Key pressing time

		public:
			// Constructor
			Key(KeyboardKey key = 0);

			// Equals operator
			bool operator==(const Key& other) const;

			// Equals operator
			bool operator==(KeyboardKey key) const;
		};

	protected:
		KeysVec    mPressedKeys;         // Pressed keys at current frame
		KeysVec    mDownKeys;            // Held down at current frame keys
		KeysVec    mReleasedKeys;        // Released at current frame keys

		CursorsVec mCursors;             // Cursors. First - main cursor
		CursorsVec mReleasedCursors;     // Released cursors. First - main cursor
		Vec2F      mMainCursorPos;       // Main cursor position
		Vec2F      mMainCursorDelta;     // Main cursor delta

		float      mMouseWheelDelta = 0; // Mouse wheel delta at current frame

	protected:
		// Call it when key pressed
		void KeyPressed(KeyboardKey key);

		// Call it when key released
		void KeyReleased(KeyboardKey key);

		// Call it when cursor pressed
		int CursorPressed(const Vec2F& pos);

		// Call it when cursor changed position. Id - index of cursor*/
		void SetCursorPos(const Vec2F& pos, CursorId id = 0, bool withDelta = true);

		// Call it when cursor released
		void CursorReleased(CursorId id = 0);

		// Call it when alt cursor pressed (right mouse button)
		void AltCursorPressed(const Vec2F& pos);

		// Call it when alt cursor released (right mouse button)
		void AltCursorReleased();

		// Call it when alt 2 cursor pressed (medium mouse button)
		void Alt2CursorPressed(const Vec2F& pos);

		// Call it when alt 2cursor released (medium mouse button)
		void Alt2CursorReleased();

		// Call when changed mouse wheel delta
		void SetMouseWheelDelta(float delta);

		// Returns true, when cursor pressed at current frame
		bool IsMainCursorPressed();

		// Returns true, when cursor down at current frame
		bool IsMainCursorDown();

		// Returns true, when cursor released at current frame
		bool IsMainCursorReleased();

		// Returns main cursor position
		Vec2F GetMainCursorPos();

		// Returns main cursor delta
		Vec2F GetMainCursorDelta();

		friend class Application;
		friend class WndProcFunc;
	};
}
