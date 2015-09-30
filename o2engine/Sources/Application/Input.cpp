#include "Input.h"

#include "Utils/Time.h"
#include "Config/ProjectConfig.h"

namespace o2
{
	DECLARE_SINGLETON(Input);

	Input::Input()
	{
		if (PLATFORM == ProjectConfig::Platform::Windows)
		{
			mCursors.Add(Cursor());
			mCursors.Last().mPressed = false;
		}

		InitializeProperties();
	}

	bool Input::IsKeyPressed(KeyboardKey key) const
	{
		for (auto ikey : mPressedKeys)
		{
			if (ikey.mKey == key)
				return true;
		}

		return false;
	}

	bool Input::IsKeyDown(KeyboardKey key) const
	{
		for (auto ikey : mDownKeys)
		{
			if (ikey.mKey == key)
				return true;
		}

		return IsKeyPressed(key);
	}

	bool Input::IsKeyReleased(KeyboardKey key) const
	{
		for (auto ikey : mReleasedKeys)
		{
			if (ikey.mKey == key)
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
		for (auto ikey : mDownKeys)
		{
			if (ikey.mKey == key)
				return ikey.mPressedTime;
		}

		return 0;
	}

	Vec2F Input::GetCursorPos(CursorId id /*= 0*/) const
	{
		for (auto cursor : mCursors)
		{
			if (cursor.mId == id)
				return cursor.mPosition;
		}

		if (id == 0)
			return mMainCursorPos;

		return Vec2F();
	}

	bool Input::IsCursorPressed(CursorId id /*= 0*/) const
	{
		for (auto cursor : mCursors)
		{
			if (cursor.mId == id && cursor.mPressedTime < FLT_EPSILON && cursor.mPressed)
				return true;
		}

		return false;
	}

	bool Input::IsCursorDown(CursorId id /*= 0*/) const
	{
		for (auto cursor : mCursors)
		{
			if (cursor.mId == id && cursor.mPressed)
				return true;
		}

		return false;
	}

	bool Input::IsCursorReleased(CursorId id /*= 0*/) const
	{
		for (auto cursor : mReleasedCursors)
		{
			if (cursor.mId == id)
				return true;
		}

		return false;
	}

	float Input::GetCursorPressingTime(CursorId id /*= 0*/) const
	{
		for (auto cursor : mCursors)
		{
			if (cursor.mId == id)
				return cursor.mPressedTime;
		}

		return 0;
	}

	Vec2F Input::GetCursorDelta(CursorId id /*= 0*/) const
	{
		for (auto cursor : mCursors)
		{
			if (cursor.mId == id)
			{
				return cursor.mDelta;
			}
		}

		if (id == 0)
			return mMainCursorDelta;

		return Vec2F();
	}

	bool Input::IsRightMousePressed() const
	{
		return IsKeyPressed((KeyboardKey)-1);
	}

	bool Input::IsRightMouseDown() const
	{
		return IsKeyDown((KeyboardKey)-1);
	}

	bool Input::IsRightMouseReleased() const
	{
		return IsKeyReleased((KeyboardKey)-1);
	}

	float Input::GetRightMousePressedTime() const
	{
		return GetKeyPressingTime((KeyboardKey)-1);
	}

	bool Input::IsMiddleMousePressed() const
	{
		return IsKeyPressed((KeyboardKey)-2);
	}

	bool Input::IsMiddleMouseDown() const
	{
		return IsKeyDown((KeyboardKey)-2);
	}

	bool Input::IsMiddleMouseReleased() const
	{
		return IsKeyReleased((KeyboardKey)-2);
	}

	float Input::GetMiddleMousePressedTime() const
	{
		return GetKeyPressingTime((KeyboardKey)-2);
	}

	float Input::GetMouseWheelDelta() const
	{
		return mMouseWheelDelta;
	}

	Ptr<Input::Cursor> Input::GetCursor(CursorId id)
	{
		for (auto& cursor : mCursors)
			if (cursor.mId == id)
				return &cursor;

		return nullptr;
	}

	const Input::CursorsVec& Input::GetCursors() const
	{
		return mCursors;
	}

	const Input::CursorsVec& Input::GetReleasedCursors() const
	{
		return mReleasedCursors;
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
		for (auto ikey : mDownKeys)
		{
			if (ikey.mKey == key)
			{
				mDownKeys.Remove(ikey);
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
				if (it.mId == id && it.mPressed)
				{
					success = false;
					break;
				}
			}

			if (success)
				break;
			else
				id++;
		}

		if (id == 0 && o2Config.GetPlatform() == ProjectConfig::Platform::Windows)
		{
			mCursors[0].mPosition = pos;
			mCursors[0].mPressed = true;
			mCursors[0].mPressedTime = 0.0f;
		}
		else mCursors.Add(Cursor(pos, id));

		return id;
	}

	void Input::CursorReleased(CursorId id /*= 0*/)
	{
		Cursor releasedCuros(Vec2F(), -100);
		for (auto& cursor : mCursors)
		{
			if (cursor.mId == id)
			{
				releasedCuros = cursor;

				if (id == 0 && o2Config.GetPlatform() == ProjectConfig::Platform::Windows)
					cursor.mPressed = false;
				else
					mCursors.Remove(cursor);

				break;
			}
		}

		mReleasedCursors.Add(releasedCuros);
	}

	void Input::SetCursorPos(const Vec2F& pos, CursorId id /*= 0*/)
	{
		for (Cursor& cursor : mCursors)
		{
			if (cursor.mId == id)
			{
				cursor.mDelta = pos - cursor.mPosition;
				cursor.mPosition = pos;
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

		for (Key& ikey : mDownKeys)
			ikey.mPressedTime += dt;

		for (Cursor& cursor : mCursors)
		{
			cursor.mPressedTime += dt;
			cursor.mDelta = Vec2F();
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
		INITIALIZE_GETTER(Input, altCursorPressed, IsRightMousePressed);
		INITIALIZE_GETTER(Input, altCursorDown, IsRightMouseDown);
		INITIALIZE_GETTER(Input, altCursorReleased, IsRightMouseReleased);
		INITIALIZE_GETTER(Input, alt2CursorPressed, IsMiddleMousePressed);
		INITIALIZE_GETTER(Input, alt2CursorDown, IsMiddleMouseDown);
		INITIALIZE_GETTER(Input, alt2CursorReleased, IsMiddleMouseReleased);
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

	Input::Cursor::Cursor(const Vec2F& position /*= Vec2F()*/, CursorId id /*= 0*/):
		mPosition(position), mDelta(), mId(id), mPressedTime(0), mPressed(true)
	{}


	bool Input::Key::operator==(const Key& other)
	{
		return mKey == other.mKey && Math::Equals(other.mPressedTime, mPressedTime);
	}

	Input::Key::Key(KeyboardKey key /*= 0*/):mKey(key), mPressedTime(0)
	{}
}