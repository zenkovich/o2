#include "Input.h"

#include "Utils/Time.h"

namespace o2
{
	DECLARE_SINGLETON(InputStuff);

	InputStuff::InputStuff():
		mMouseWheelDelta(0.0f)
	{
	}

	bool InputStuff::IsKeyPressed(KeyboardKey key) const
	{
		for (auto it : mInstance->mPressedKeys)
		{
			if (it.mKey == key)
				return true;
		}

		return false;
	}

	bool InputStuff::IsKeyDown(KeyboardKey key) const
	{
		for (auto it : mInstance->mDownKeys)
		{
			if (it.mKey == key)
				return true;
		}

		return IsKeyPressed(key);
	}

	bool InputStuff::IsKeyReleased(KeyboardKey key) const
	{
		for (auto it : mInstance->mReleasedKeys)
		{
			if (it.mKey == key)
				return true;
		}

		return false;
	}

	bool InputStuff::IsKeyRepeating(KeyboardKey key, float delay /*= 0.1f*/, float beginDelay /*= 0.5f*/) const
	{
		float time = GetKeyPressingTime(key) - beginDelay;
		float repeatTime = Math::Max(floor(time/delay)*delay, 0.0f);

		return time - o2Time.GetDeltaTime() < repeatTime && time > repeatTime;
	}

	float InputStuff::GetKeyPressingTime(KeyboardKey key) const
	{
		for (auto it : mInstance->mDownKeys)
		{
			if (it.mKey == key)
				return it.mPressedTime;
		}

		return 0;
	}

	Vec2F InputStuff::GetCursorPos(int id /*= 0*/) const
	{
		for (auto it : mInstance->mCursors)
		{
			if (it.mId == id)
				return it.mPosition;
		}

		if (id == 0)
			return mInstance->mMainCursorPos;

		return Vec2F();
	}

	bool InputStuff::IsCursorPressed(int id /*= 0*/) const
	{
		for (auto it : mInstance->mCursors)
		{
			if (it.mId == id && it.mPressedTime < FLT_EPSILON)
				return true;
		}

		return false;
	}

	bool InputStuff::IsCursorDown(int id /*= 0*/) const
	{
		for (auto it : mInstance->mCursors)
		{
			if (it.mId == id)
				return true;
		}

		return false;
	}

	bool InputStuff::IsCursorReleased(int id /*= 0*/) const
	{
		for (auto it : mInstance->mReleasedCursors)
		{
			if (it.mId == id)
				return true;
		}

		return false;
	}

	float InputStuff::GetCursorPressingTime(int id /*= 0*/) const
	{
		for (auto it : mInstance->mCursors)
		{
			if (it.mId == id)
				return it.mPressedTime;
		}

		return 0;
	}

	Vec2F InputStuff::GetCursorDelta(int id /*= 0*/) const
	{
		for (auto it : mInstance->mCursors)
		{
			if (it.mId == id)
				return it.mDelta;
		}

		if (id == 0)
			return mInstance->mMainCursorDelta;

		return Vec2F();
	}

	bool InputStuff::IsAltCursorPressed() const
	{
		return IsKeyPressed((KeyboardKey)-1);
	}

	bool InputStuff::IsAltCursorDown() const
	{
		return IsKeyDown((KeyboardKey)-1);
	}

	bool InputStuff::IsAltCursorReleased() const
	{
		return IsKeyReleased((KeyboardKey)-1);
	}

	float InputStuff::GetAltCursorPressedTime() const
	{
		return GetKeyPressingTime((KeyboardKey)-1);
	}

	bool InputStuff::IsAlt2CursorPressed() const
	{
		return IsKeyPressed((KeyboardKey)-2);
	}

	bool InputStuff::IsAlt2CursorDown() const
	{
		return IsKeyDown((KeyboardKey)-2);
	}

	bool InputStuff::IsAlt2CursorReleased() const
	{
		return IsKeyReleased((KeyboardKey)-2);
	}

	float InputStuff::GetAlt2CursorPressedTime() const
	{
		return GetKeyPressingTime((KeyboardKey)-2);
	}

	float InputStuff::GetMouseWheelDelta() const
	{
		return mInstance->mMouseWheelDelta;
	}

	InputStuff::CursorsVec const& InputStuff::GetCursors() const
	{
		return mInstance->mCursors;
	}

	InputStuff::KeysVec const& InputStuff::GetPressedKeys() const
	{
		return mInstance->mPressedKeys;
	}

	InputStuff::KeysVec const& InputStuff::GetDownKeys() const
	{
		return mInstance->mDownKeys;
	}

	InputStuff::KeysVec const& InputStuff::GetReleasedKeys() const
	{
		return mInstance->mReleasedKeys;
	}


	void InputStuff::KeyPressed(KeyboardKey key)
	{
		if (IsKeyDown(key))
			return;

		mPressedKeys.Add(key);
	}

	void InputStuff::KeyReleased(KeyboardKey key)
	{
		for (auto it : mDownKeys)
		{
			if (it.mKey == key)
			{
				mDownKeys.Remove(it);
				break;
			}
		}
		mReleasedKeys.Add(key);
	}

	int InputStuff::CursorPressed(const Vec2F& pos)
	{
		//find free id
		int id = 0;
		while (id < 100)
		{
			bool success = true;
			for (auto it : mCursors)
			{
				if (it.mId == id)
				{
					success =false;
					break;
				}
			}

			if (success)
				break;
			else
				id++;
		}

		mCursors.Add(Cursor(pos, id));

		return id;
	}

	void InputStuff::CursorReleased(int id /*= 0*/)
	{
		Cursor releasedCuros(Vec2F(), -100);
		for (auto it : mCursors)
		{
			if (it.mId == id)
			{
				releasedCuros = it;
				mCursors.Remove(it);
				break;
			}
		}
		mReleasedCursors.Add(releasedCuros);
	}

	void InputStuff::SetCursorPos(const Vec2F& pos, int id /*= 0*/)
	{
		for (auto it : mCursors)
		{
			if (it.mId == id)
			{
				it.mDelta = pos - it.mPosition;
				it.mPosition = pos;
				break;
			}
		}

		if (id == 0)
		{
			mMainCursorDelta = pos - mMainCursorPos;
			mMainCursorPos = pos;
		}
	}

	void InputStuff::update(float dt)
	{
		mDownKeys.Add(mPressedKeys);
		mPressedKeys.Clear();
		mReleasedKeys.Clear();

		for (auto it : mDownKeys)
			it.mPressedTime += dt;

		for (auto it : mCursors)
		{
			it.mPressedTime += dt;
			it.mDelta = Vec2F();
		}

		mMainCursorDelta = Vec2F();

		mReleasedCursors.Clear();

		mMouseWheelDelta = 0;
	}

	void InputStuff::AltCursorPressed(const Vec2F& pos)
	{
		KeyPressed(-1);
		SetCursorPos(pos);
	}

	void InputStuff::AltCursorReleased()
	{
		KeyReleased(-1);
	}

	void InputStuff::Alt2CursorPressed(const Vec2F& pos)
	{
		KeyPressed(-2);
		SetCursorPos(pos);
	}

	void InputStuff::Alt2CursorReleased()
	{
		KeyReleased(-2);
	}

	void InputStuff::SetMouseWheelDelta(float delta)
	{
		mMouseWheelDelta = delta;
	}


	bool InputStuff::Cursor::operator==(const Cursor& other)
	{
		return Math::Equals(mPosition, other.mPosition) && Math::Equals(mDelta, other.mDelta) && mId == other.mId &&
			Math::Equals(mPressedTime, other.mPressedTime);
	}

	InputStuff::Cursor::Cursor(const Vec2F& position /*= Vec2F()*/, int id /*= 0*/):mPosition(position), mDelta(), mId(id), mPressedTime(0)
	{
	}


	bool InputStuff::Key::operator==(const Key& other)
	{
		return mKey == other.mKey && Math::Equals(other.mPressedTime, mPressedTime);
	}

	InputStuff::Key::Key(KeyboardKey key /*= 0*/):mKey(key), mPressedTime(0)
	{
	}
}