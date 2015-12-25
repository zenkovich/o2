#pragma once

#include "public.h"
#include "util/math/vector2.h"

OPEN_O2_NAMESPACE

/** Virtual key. */
typedef int VKey;

/** Input message. Containing pressed, down, released keys, cursors positions. */
class InputMessage
{
	friend class Application;
	friend class ApplicationBaseInterface;
	friend class InputMsgTest;

public:
	/** Cursor definition. */
	struct Cursor
	{
		Vec2F mPosition;
		Vec2F mDelta;
		int   mId;
		float mPressedTime;

		Cursor(const Vec2F& position = Vec2F(), int id = 0):mPosition(position), mDelta(), mId(id), mPressedTime(0) {}

		bool operator==(const Cursor& other)
		{
			return Equals(mPosition, other.mPosition) && Equals(mDelta, other.mDelta) && mId == other.mId &&
				Equals(mPressedTime, other.mPressedTime);
		}
	};
	typedef Array<Cursor> CursorsArr;

	/** Key with pressed time. */
	struct Key
	{
		VKey  mKey;
		float mPressedTime;

		Key(VKey key = 0):mKey(key), mPressedTime(0) {}

		bool operator==(const Key& other)
		{
			return mKey == other.mKey && Equals(other.mPressedTime, mPressedTime);
		}
	};

	typedef Array<Key> KeysArr;

protected:
	KeysArr    mPressedKeys;
	KeysArr    mDownKeys;
	KeysArr    mReleasedKeys;

	CursorsArr mCursors;         /**< Cursors. First - main cursor. */
	CursorsArr mReleasedCursors; /**< Released cursors. First - main cursor. */
	Vec2F      mMainCursorPos;   /**< Main cursor position. */
	Vec2F      mMainCursorDelta; /**< Main cursor delta. */
	float      mMouseWheelDelta; /** Mouse wheel delta at current frame. */

public:
	/** ctor. */
	InputMessage();

	/** Returns true if key was pressed at current frame. */
	bool IsKeyPressed(VKey key) const;

	/** Returns true if key was pressed some frames ago. */
	bool IsKeyDown(VKey key) const;

	/** Returns true, if key was released at current frame. */
	bool IsKeyReleased(VKey key) const;

	/** Returns true, if key was pressed and repeated after delay. */
	bool IsKeyRepeating(VKey key, float delay = 0.04f, float beginDelay = 0.5f) const;

	/** Returns key pressing time.Returns  0, if key not pressed. */
	float GetKeyPressingTime(VKey key) const;

	/** Returns position of cursor. */
	Vec2F GetCursorPos(int id = 0) const;

	/** Returns true, when cursor pressed at current frame. */
	bool IsCursorPressed(int id = 0) const;

	/** Returns true, when cursor is down. */
	bool IsCursorDown(int id = 0) const;

	/** Returns true, when cursor released at current frame. */
	bool IsCursorReleased(int id = 0) const;

	/** Returns cursor pressed time. 0 if cursor not pressed. */
	float GetCursorPressingTime(int id = 0) const;

	/** Returns cursor delta. */
	Vec2F GetCursorDelta(int id = 0) const;

	/** Returns true, when alt cursor pressed (right mouse button). */
	bool IsAltCursorPressed() const;

	/** Returns true, when alt cursor down (right mouse button). */
	bool IsAltCursorDown() const;

	/** Returns true, when alt cursor released (right mouse button). */
	bool IsAltCursorReleased() const;

	/** Returns alt cursor pressed time (right mouse button). 0 if cursor not pressed. */
	float GetAltCursorPressedTime() const;

	/** Returns true, when alt 2 cursor pressed (medium mouse button). */
	bool IsAlt2CursorPressed() const;

	/** Returns true, when alt 2 cursor down (medium mouse button). */
	bool IsAlt2CursorDown() const;

	/** Returns true, when alt 2 cursor released (medium mouse button). */
	bool IsAlt2CursorReleased() const;

	/** Returns alt 2 cursor pressed time (medium mouse button). 0 if cursor not pressed. */
	float GetAlt2CursorPressedTime() const;

	/** Returns mouse wheel delta at frame. */
	float GetMouseWheelDelta() const;

	/** Returns cursors vector. */
	CursorsArr const& GetCursors() const;

	/** Returns pressed keys. */
	KeysArr const& GetPressedKeys() const;

	/** Returns pressed keys. */
	KeysArr const& GetDownKeys() const;

	/** Returns pressed keys. */
	KeysArr const& GetReleasedKeys() const;

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

CLOSE_O2_NAMESPACE