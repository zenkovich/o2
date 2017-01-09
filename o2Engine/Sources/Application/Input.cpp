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
			mCursors.Last().isPressed = false;
		}

		InitializeProperties();
	}

	bool Input::IsKeyPressed(KeyboardKey key) const
	{
		for (auto ikey : mPressedKeys)
		{
			if (ikey.keyCode == key)
				return true;
		}

		return false;
	}

	bool Input::IsKeyDown(KeyboardKey key) const
	{
		for (auto ikey : mDownKeys)
		{
			if (ikey.keyCode == key)
				return true;
		}

		return IsKeyPressed(key);
	}

	bool Input::IsKeyReleased(KeyboardKey key) const
	{
		for (auto ikey : mReleasedKeys)
		{
			if (ikey.keyCode == key)
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
			if (ikey.keyCode == key)
				return ikey.pressedTime;
		}

		return 0;
	}

	Vec2F Input::GetCursorPos(CursorId id /*= 0*/) const
	{
		for (auto cursor : mCursors)
		{
			if (cursor.id == id)
				return cursor.position;
		}

		if (id == 0)
			return mMainCursorPos;

		return Vec2F();
	}

	bool Input::IsCursorPressed(CursorId id /*= 0*/) const
	{
		for (auto cursor : mCursors)
		{
			if (cursor.id == id && cursor.pressedTime < FLT_EPSILON && cursor.isPressed)
				return true;
		}

		return false;
	}

	bool Input::IsCursorDown(CursorId id /*= 0*/) const
	{
		for (auto cursor : mCursors)
		{
			if (cursor.id == id && cursor.isPressed)
				return true;
		}

		return false;
	}

	bool Input::IsCursorReleased(CursorId id /*= 0*/) const
	{
		for (auto cursor : mReleasedCursors)
		{
			if (cursor.id == id)
				return true;
		}

		return false;
	}

	float Input::GetCursorPressingTime(CursorId id /*= 0*/) const
	{
		for (auto cursor : mCursors)
		{
			if (cursor.id == id)
				return cursor.pressedTime;
		}

		return 0;
	}

	Vec2F Input::GetCursorDelta(CursorId id /*= 0*/) const
	{
		for (auto cursor : mCursors)
		{
			if (cursor.id == id)
			{
				return cursor.delta;
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

	Input::Cursor* Input::GetCursor(CursorId id)
	{
		for (auto& cursor : mCursors)
			if (cursor.id == id)
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
		if (IsKeyDown(key) || IsKeyPressed(key))
			return;

		mPressedKeys.Add(key);
	}

	void Input::KeyReleased(KeyboardKey key)
	{
		for (auto ikey : mDownKeys)
		{
			if (ikey.keyCode == key)
			{
				mDownKeys.Remove(ikey);
				mReleasedKeys.Add(key);
				return;
			}
		}

		for (auto ikey : mPressedKeys)
		{
			if (ikey.keyCode == key)
			{
				mPressedKeys.Remove(ikey);
				return;
			}
		}
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
				if (it.id == id && it.isPressed)
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
			mCursors[0].position = pos;
			mCursors[0].isPressed = true;
			mCursors[0].pressedTime = 0.0f;
		}
		else mCursors.Add(Cursor(pos, id));

		return id;
	}

	void Input::CursorReleased(CursorId id /*= 0*/)
	{
		Cursor releasedCuros(Vec2F(), -100);
		for (auto& cursor : mCursors)
		{
			if (cursor.id == id)
			{
				releasedCuros = cursor;

				if (id == 0 && o2Config.GetPlatform() == ProjectConfig::Platform::Windows)
					cursor.isPressed = false;
				else
					mCursors.Remove(cursor);

				break;
			}
		}

		mReleasedCursors.Add(releasedCuros);
	}

	void Input::SetCursorPos(const Vec2F& pos, CursorId id /*= 0*/, bool withDelta /*= true*/)
	{
		for (Cursor& cursor : mCursors)
		{
			if (cursor.id == id)
			{
				if (withDelta)
					cursor.delta += pos - cursor.position;

				cursor.position = pos;
				break;
			}
		}

		if (id == 0)
		{
			if (withDelta)
				mMainCursorDelta += pos - mMainCursorPos;

			mMainCursorPos = pos;
		}
	}

	void Input::Update(float dt)
	{
		mDownKeys.Add(mPressedKeys);
		mPressedKeys.Clear();
		mReleasedKeys.Clear();

		for (Key& ikey : mDownKeys)
			ikey.pressedTime += dt;

		for (Cursor& cursor : mCursors)
		{
			cursor.pressedTime += dt;
			cursor.delta = Vec2F();
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
		return Math::Equals(position, other.position) && Math::Equals(delta, other.delta) && id == other.id &&
			Math::Equals(pressedTime, other.pressedTime);
	}

	Input::Cursor::Cursor(const Vec2F& position /*= Vec2F()*/, CursorId id /*= 0*/):
		position(position), delta(), id(id), pressedTime(0), isPressed(true)
	{}


	bool Input::Key::operator==(const Key& other) const
	{
		return keyCode == other.keyCode && Math::Equals(other.pressedTime, pressedTime);
	}

	Input::Key::Key(KeyboardKey key /*= 0*/):keyCode(key), pressedTime(0)
	{}

	bool Input::Key::operator==(KeyboardKey key) const
	{
		return keyCode == key;
	}

}