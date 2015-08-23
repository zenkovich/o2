#pragma once

#include "Utils/Containers/Vector.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Singleton.h"

namespace o2
{
	/** Virtual key. */
	typedef int VKey;

	/** Input message. Containing pressed, down, released keys, cursors positions. */
	class Input: public Singleton<Input>
	{
		friend class Application;
		friend class IApplication;

	public:
		/** Cursor definition. */
		struct Cursor
		{
			Vec2F mPosition;
			Vec2F mDelta;
			int   mId;
			float mPressedTime;

		public:
			Cursor(const Vec2F& position = Vec2F(), int id = 0);
			bool operator==(const Cursor& other);
		};
		typedef Vector<Cursor> CursorsVec;

		/** Key with pressed time. */
		struct Key
		{
			VKey  mKey;
			float mPressedTime;

		public:
			Key(VKey key = 0);
			bool operator==(const Key& other);
		};

		typedef Vector<Key> KeysVec;

	protected:
		KeysVec    mPressedKeys;
		KeysVec    mDownKeys;
		KeysVec    mReleasedKeys;

		CursorsVec mCursors;         /**< Cursors. First - main cursor. */
		CursorsVec mReleasedCursors; /**< Released cursors. First - main cursor. */
		Vec2F      mMainCursorPos;   /**< Main cursor position. */
		Vec2F      mMainCursorDelta; /**< Main cursor delta. */
		float      mMouseWheelDelta; /**< Mouse wheel delta at current frame. */

	public:
		/** ctor. */
		Input();

		/** Returns true if key was pressed at current frame. */
		static bool IsKeyPressed(VKey key);

		/** Returns true if key was pressed some frames ago. */
		static bool IsKeyDown(VKey key);

		/** Returns true, if key was released at current frame. */
		static bool IsKeyReleased(VKey key);

		/** Returns true, if key was pressed and repeated after delay. */
		static bool IsKeyRepeating(VKey key, float delay = 0.04f, float beginDelay = 0.5f);

		/** Returns key pressing time.Returns  0, if key not pressed. */
		static float GetKeyPressingTime(VKey key);

		/** Returns position of cursor. */
		static Vec2F GetCursorPos(int id = 0);

		/** Returns true, when cursor pressed at current frame. */
		static bool IsCursorPressed(int id = 0);

		/** Returns true, when cursor is down. */
		static bool IsCursorDown(int id = 0);

		/** Returns true, when cursor released at current frame. */
		static bool IsCursorReleased(int id = 0);

		/** Returns cursor pressed time. 0 if cursor not pressed. */
		static float GetCursorPressingTime(int id = 0);

		/** Returns cursor delta. */
		static Vec2F GetCursorDelta(int id = 0);

		/** Returns true, when alt cursor pressed (right mouse button). */
		static bool IsAltCursorPressed();

		/** Returns true, when alt cursor down (right mouse button). */
		static bool IsAltCursorDown();

		/** Returns true, when alt cursor released (right mouse button). */
		static bool IsAltCursorReleased();

		/** Returns alt cursor pressed time (right mouse button). 0 if cursor not pressed. */
		static float GetAltCursorPressedTime();

		/** Returns true, when alt 2 cursor pressed (medium mouse button). */
		static bool IsAlt2CursorPressed();

		/** Returns true, when alt 2 cursor down (medium mouse button). */
		static bool IsAlt2CursorDown();

		/** Returns true, when alt 2 cursor released (medium mouse button). */
		static bool IsAlt2CursorReleased();

		/** Returns alt 2 cursor pressed time (medium mouse button). 0 if cursor not pressed. */
		static float GetAlt2CursorPressedTime();

		/** Returns mouse wheel delta at frame. */
		static float GetMouseWheelDelta();

		/** Returns cursors vector. */
		static CursorsVec const& GetCursors();

		/** Returns pressed keys. */
		static KeysVec const& GetPressedKeys();

		/** Returns pressed keys. */
		static KeysVec const& GetDownKeys();

		/** Returns pressed keys. */
		static KeysVec const& GetReleasedKeys();

	protected:
		/** Call it when key pressed. */
		void KeyPressed(VKey key);

		/** Call it when key released. */
		void KeyReleased(VKey key);

		/** Call it when cursor pressed. */
		int CursorPressed(const Vec2F& pos);

		/** Call it when cursor changed position. Id - index of cursor*/
		void SetCursorPos(const Vec2F& pos, int id = 0);

		/** Call it when cursor released. */
		void CursorReleased(int id = 0);

		/** Call it when alt cursor pressed (right mouse button). */
		void AltCursorPressed(const Vec2F& pos);

		/** Call it when alt cursor released (right mouse button). */
		void AltCursorReleased();

		/** Call it when alt 2 cursor pressed (medium mouse button). */
		void Alt2CursorPressed(const Vec2F& pos);

		/** Call it when alt 2cursor released (medium mouse button). */
		void Alt2CursorReleased();

		/** Call when changed mouse wheel delta. */
		void SetMouseWheelDelta(float delta);

		/** Call it after frame update. */
		void update(float dt);
	};
}