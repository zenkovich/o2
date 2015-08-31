#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Singleton.h"

namespace o2
{
	// Keyboard key
	typedef int KeyboardKey;

// Input access macros
#define Input InputStuff::Instance()

	// -------------------------------------------------------------------------
	// Input message. Containing pressed, down, released keys, cursors positions
	// -------------------------------------------------------------------------
	class InputStuff: public Singleton<InputStuff>
	{
		friend class BaseApplication;
		friend class IApplication;

	public:
		// -----------------
		// Cursor definition
		// -----------------
		struct Cursor
		{
			Vec2F mPosition;    // Current cursor position, in pixels, from left top corner 
			Vec2F mDelta;       // Cursor moving delta between frames
			int   mId;          // Cursor id
			float mPressedTime; // Time until key is pressed in seconds

		public:
			// Constructor
			Cursor(const Vec2F& position = Vec2F(), int id = 0);

			// Equals operator
			bool operator==(const Cursor& other);
		};
		typedef Vector<Cursor> CursorsVec;

		// ---------------------
		// Key with pressed time
		// ---------------------
		struct Key
		{
			KeyboardKey mKey;         // Key id
			float       mPressedTime; // Key pressing time

		public:
			// Constructor
			Key(KeyboardKey key = 0);

			// Equals operator
			bool operator==(const Key& other);
		};

		typedef Vector<Key> KeysVec;

	protected:
		KeysVec    mPressedKeys;     // Pressed keys at current frame
		KeysVec    mDownKeys;        // Held down at current frame keys
		KeysVec    mReleasedKeys;    // Released at current frame keys

		CursorsVec mCursors;         // Cursors. First - main cursor
		CursorsVec mReleasedCursors; // Released cursors. First - main cursor
		Vec2F      mMainCursorPos;   // Main cursor position
		Vec2F      mMainCursorDelta; // Main cursor delta
		float      mMouseWheelDelta; // Mouse wheel delta at current frame

	public:
		// Default constructor
		InputStuff();

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
		Vec2F GetCursorPos(int id = 0) const;

		// Returns true, when cursor pressed at current frame
		bool IsCursorPressed(int id = 0) const;

		// Returns true, when cursor is down
		bool IsCursorDown(int id = 0) const;

		// Returns true, when cursor released at current frame
		bool IsCursorReleased(int id = 0) const;

		// Returns cursor pressed time. 0 if cursor not pressed
		float GetCursorPressingTime(int id = 0) const;

		// Returns cursor delta
		Vec2F GetCursorDelta(int id = 0) const;

		// Returns true, when alt cursor pressed (right mouse button)
		bool IsAltCursorPressed() const;

		// Returns true, when alt cursor down (right mouse button)
		bool IsAltCursorDown() const;

		// Returns true, when alt cursor released (right mouse button)
		bool IsAltCursorReleased() const;

		// Returns alt cursor pressed time (right mouse button). 0 if cursor not pressed
		float GetAltCursorPressedTime() const;

		// Returns true, when alt 2 cursor pressed (medium mouse button)
		bool IsAlt2CursorPressed() const;

		// Returns true, when alt 2 cursor down (medium mouse button)
		bool IsAlt2CursorDown() const;

		// Returns true, when alt 2 cursor released (medium mouse button)
		bool IsAlt2CursorReleased() const;

		// Returns alt 2 cursor pressed time (medium mouse button). 0 if cursor not pressed
		float GetAlt2CursorPressedTime() const;

		// Returns mouse wheel delta at frame
		float GetMouseWheelDelta() const;

		// Returns cursors vector
		CursorsVec const& GetCursors() const;

		// Returns pressed keys
		KeysVec const& GetPressedKeys() const;

		// Returns pressed keys
		KeysVec const& GetDownKeys() const;

		// Returns pressed keys
		KeysVec const& GetReleasedKeys() const;

	protected:
		// Call it when key pressed
		void KeyPressed(KeyboardKey key);

		// Call it when key released
		void KeyReleased(KeyboardKey key);

		// Call it when cursor pressed
		int CursorPressed(const Vec2F& pos);

		// Call it when cursor changed position. Id - index of cursor*/
		void SetCursorPos(const Vec2F& pos, int id = 0);

		// Call it when cursor released
		void CursorReleased(int id = 0);

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

		// Call it after frame update
		void update(float dt);
	};
}