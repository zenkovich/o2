#include "Input.h"

#include "Utils/Time.h"

namespace o2
{
	DECLARE_SINGLETON(Input);

	Input::Input()
	{
		InitializeProperties();
	}

	bool Input::IsKeyPressed(KeyboardKey key) const
	{
		for (auto it : mPressedKeys)
		{
			if (it.mKey == key)
				return true;
		}

		return false;
	}

	bool Input::IsKeyDown(KeyboardKey key) const
	{
		for (auto it : mDownKeys)
		{
			if (it.mKey == key)
				return true;
		}

		return IsKeyPressed(key);
	}

	bool Input::IsKeyReleased(KeyboardKey key) const
	{
		for (auto it : mReleasedKeys)
		{
			if (it.mKey == key)
				return true;
		}

		return false;
	}

	bool Input::IsKeyRepeating(KeyboardKey key, float delay /*= 0.1f*/, float beginDelay /*= 0.5f*/) const
	{
		float time = GetKeyPressingTime(key) - beginDelay;
		float repeatTime = Math::Max(floor(time/delay)*delay, 0.0f);

		return time - o2Time.GetDeltaTime() < repeatTime && time > repeatTime;
	}

	float Input::GetKeyPressingTime(KeyboardKey key) const
	{
		for (auto it : mDownKeys)
		{
			if (it.mKey == key)
				return it.mPressedTime;
		}

		return 0;
	}

	Vec2F Input::GetCursorPos(int id /*= 0*/) const
	{
		for (auto it : mCursors)
		{
			if (it.mId == id)
				return it.mPosition;
		}

		if (id == 0)
			return mMainCursorPos;

		return Vec2F();
	}

	bool Input::IsCursorPressed(int id /*= 0*/) const
	{
		for (auto it : mCursors)
		{
			if (it.mId == id && it.mPressedTime < FLT_EPSILON)
				return true;
		}

		return false;
	}

	bool Input::IsCursorDown(int id /*= 0*/) const
	{
		for (auto it : mCursors)
		{
			if (it.mId == id)
				return true;
		}

		return false;
	}

	bool Input::IsCursorReleased(int id /*= 0*/) const
	{
		for (auto it : mReleasedCursors)
		{
			if (it.mId == id)
				return true;
		}

		return false;
	}

	float Input::GetCursorPressingTime(int id /*= 0*/) const
	{
		for (auto it : mCursors)
		{
			if (it.mId == id)
				return it.mPressedTime;
		}

		return 0;
	}

	Vec2F Input::GetCursorDelta(int id /*= 0*/) const
	{
		for (auto it : mCursors)
		{
			if (it.mId == id)
			{
				return it.mDelta;
			}
		}

		if (id == 0)
			return mMainCursorDelta;

		return Vec2F();
	}

	bool Input::IsAltCursorPressed() const
	{
		return IsKeyPressed((KeyboardKey)-1);
	}

	bool Input::IsAltCursorDown() const
	{
		return IsKeyDown((KeyboardKey)-1);
	}

	bool Input::IsAltCursorReleased() const
	{
		return IsKeyReleased((KeyboardKey)-1);
	}

	float Input::GetAltCursorPressedTime() const
	{
		return GetKeyPressingTime((KeyboardKey)-1);
	}

	bool Input::IsAlt2CursorPressed() const
	{
		return IsKeyPressed((KeyboardKey)-2);
	}

	bool Input::IsAlt2CursorDown() const
	{
		return IsKeyDown((KeyboardKey)-2);
	}

	bool Input::IsAlt2CursorReleased() const
	{
		return IsKeyReleased((KeyboardKey)-2);
	}

	float Input::GetAlt2CursorPressedTime() const
	{
		return GetKeyPressingTime((KeyboardKey)-2);
	}

	float Input::GetMouseWheelDelta() const
	{
		return mMouseWheelDelta;
	}

	Input::CursorsVec const& Input::GetCursors() const
	{
		return mCursors;
	}

	Input::KeysVec const& Input::GetPressedKeys() const
	{
		return mPressedKeys;
	}

	Input::KeysVec const& Input::GetDownKeys() const
	{
		return mDownKeys;
	}

	Input::KeysVec const& Input::GetReleasedKeys() const
	{
		return mReleasedKeys;
	}


	void Input::KeyPressed(KeyboardKey key)
	{
		if (IsKeyDown(key))
			return;

		mPressedKeys.Add(key);
	}

	void Input::KeyReleased(KeyboardKey key)
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
		for (Cursor& it : mCursors)
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

	void Input::Update(float dt)
	{
		mDownKeys.Add(mPressedKeys);
		mPressedKeys.Clear();
		mReleasedKeys.Clear();

		for (Key& it : mDownKeys)
			it.mPressedTime += dt;

		for (Cursor& it : mCursors)
		{
			it.mPressedTime += dt;
			it.mDelta = Vec2F();
		}

		mMainCursorDelta = Vec2F();

		mReleasedCursors.Clear();

		mMouseWheelDelta = 0;
	}

	bool Input::IsMainCursorPressed()
	{
		return IsCursorPressed();
	}

	bool Input::IsMainCursorDown()
	{
		return IsCursorDown();
	}

	bool Input::IsMainCursorReleased()
	{
		return IsCursorReleased();
	}

	Vec2F Input::GetMainCursorPos()
	{
		return GetCursorPos();
	}

	Vec2F Input::GetMainCursorDelta()
	{
		return GetCursorDelta();
	}

	void Input::InitializeProperties()
	{
		INITIALIZE_GETTER(Input, cursorPressed, IsMainCursorPressed);
		INITIALIZE_GETTER(Input, cursorDown, IsMainCursorDown);
		INITIALIZE_GETTER(Input, cursorReleased, IsMainCursorReleased);
		INITIALIZE_GETTER(Input, altCursorPressed, IsAltCursorPressed);
		INITIALIZE_GETTER(Input, altCursorDown, IsAltCursorDown);
		INITIALIZE_GETTER(Input, altCursorReleased, IsAltCursorReleased);
		INITIALIZE_GETTER(Input, alt2CursorPressed, IsAlt2CursorPressed);
		INITIALIZE_GETTER(Input, alt2CursorDown, IsAlt2CursorDown);
		INITIALIZE_GETTER(Input, alt2CursorReleased, IsAlt2CursorReleased);
		INITIALIZE_GETTER(Input, cursorPos, GetMainCursorPos);
		INITIALIZE_GETTER(Input, cursorDelta, GetMainCursorDelta);
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

	Input::Key::Key(KeyboardKey key /*= 0*/):mKey(key), mPressedTime(0)
	{
	}
}