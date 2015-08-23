#include "Input.h"

#include "Utils/Time.h"

namespace o2
{
	DECLARE_SINGLETON(Input);

	Input::Input():
		mMouseWheelDelta(0.0f)
	{
	}

	bool Input::IsKeyPressed(VKey key)
	{
		for (auto it : mInstance->mPressedKeys)
		{
			if (it.mKey == key)
				return true;
		}

		return false;
	}

	bool Input::IsKeyDown(VKey key)
	{
		for (auto it : mInstance->mDownKeys)
		{
			if (it.mKey == key)
				return true;
		}

		return IsKeyPressed(key);
	}

	bool Input::IsKeyReleased(VKey key)
	{
		for (auto it : mInstance->mReleasedKeys)
		{
			if (it.mKey == key)
				return true;
		}

		return false;
	}

	bool Input::IsKeyRepeating(VKey key, float delay /*= 0.1f*/, float beginDelay /*= 0.5f*/)
	{
		float time = GetKeyPressingTime(key) - beginDelay;
		float repeatTime = Math::Max(floor(time/delay)*delay, 0.0f);

		return time - Time::GetDeltaTime() < repeatTime && time > repeatTime;
	}

	float Input::GetKeyPressingTime(VKey key)
	{
		for (auto it : mInstance->mDownKeys)
		{
			if (it.mKey == key)
				return it.mPressedTime;
		}

		return 0;
	}

	Vec2F Input::GetCursorPos(int id /*= 0*/)
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

	bool Input::IsCursorPressed(int id /*= 0*/)
	{
		for (auto it : mInstance->mCursors)
		{
			if (it.mId == id && it.mPressedTime < FLT_EPSILON)
				return true;
		}

		return false;
	}

	bool Input::IsCursorDown(int id /*= 0*/)
	{
		for (auto it : mInstance->mCursors)
		{
			if (it.mId == id)
				return true;
		}

		return false;
	}

	bool Input::IsCursorReleased(int id /*= 0*/)
	{
		for (auto it : mInstance->mReleasedCursors)
		{
			if (it.mId == id)
				return true;
		}

		return false;
	}

	float Input::GetCursorPressingTime(int id /*= 0*/)
	{
		for (auto it : mInstance->mCursors)
		{
			if (it.mId == id)
				return it.mPressedTime;
		}

		return 0;
	}

	Vec2F Input::GetCursorDelta(int id /*= 0*/)
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

	bool Input::IsAltCursorPressed()
	{
		return IsKeyPressed((VKey)-1);
	}

	bool Input::IsAltCursorDown()
	{
		return IsKeyDown((VKey)-1);
	}

	bool Input::IsAltCursorReleased()
	{
		return IsKeyReleased((VKey)-1);
	}

	float Input::GetAltCursorPressedTime()
	{
		return GetKeyPressingTime((VKey)-1);
	}

	bool Input::IsAlt2CursorPressed()
	{
		return IsKeyPressed((VKey)-2);
	}

	bool Input::IsAlt2CursorDown()
	{
		return IsKeyDown((VKey)-2);
	}

	bool Input::IsAlt2CursorReleased()
	{
		return IsKeyReleased((VKey)-2);
	}

	float Input::GetAlt2CursorPressedTime()
	{
		return GetKeyPressingTime((VKey)-2);
	}

	float Input::GetMouseWheelDelta()
	{
		return mInstance->mMouseWheelDelta;
	}

	Input::CursorsVec const& Input::GetCursors()
	{
		return mInstance->mCursors;
	}

	Input::KeysVec const& Input::GetPressedKeys()
	{
		return mInstance->mPressedKeys;
	}

	Input::KeysVec const& Input::GetDownKeys()
	{
		return mInstance->mDownKeys;
	}

	Input::KeysVec const& Input::GetReleasedKeys()
	{
		return mInstance->mReleasedKeys;
	}


	void Input::KeyPressed(VKey key)
	{
		if (IsKeyDown(key))
			return;

		mPressedKeys.Add(key);
	}

	void Input::KeyReleased(VKey key)
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

	int Input::CursorPressed(const Vec2F& pos)
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

	void Input::CursorReleased(int id /*= 0*/)
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

	void Input::SetCursorPos(const Vec2F& pos, int id /*= 0*/)
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

	void Input::update(float dt)
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

	void Input::AltCursorPressed(const Vec2F& pos)
	{
		KeyPressed(-1);
		SetCursorPos(pos);
	}

	void Input::AltCursorReleased()
	{
		KeyReleased(-1);
	}

	void Input::Alt2CursorPressed(const Vec2F& pos)
	{
		KeyPressed(-2);
		SetCursorPos(pos);
	}

	void Input::Alt2CursorReleased()
	{
		KeyReleased(-2);
	}

	void Input::SetMouseWheelDelta(float delta)
	{
		mMouseWheelDelta = delta;
	}


	bool Input::Cursor::operator==(const Cursor& other)
	{
		return Math::Equals(mPosition, other.mPosition) && Math::Equals(mDelta, other.mDelta) && mId == other.mId &&
			Math::Equals(mPressedTime, other.mPressedTime);
	}

	Input::Cursor::Cursor(const Vec2F& position /*= Vec2F()*/, int id /*= 0*/):mPosition(position), mDelta(), mId(id), mPressedTime(0)
	{
	}


	bool Input::Key::operator==(const Key& other)
	{
		return mKey == other.mKey && Math::Equals(other.mPressedTime, mPressedTime);
	}

	Input::Key::Key(VKey key /*= 0*/):mKey(key), mPressedTime(0)
	{
	}
}