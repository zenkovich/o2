#include "input_message.h"

#include "app/application.h"
#include "util/data_utils.h"
#include "util/log.h"
#include "util/math/math.h"
#include "util/time_utils.h"

#ifdef PLATFORM_WIN
#include <Windows.h>
#endif //PLATFORM_WIN

OPEN_O2_NAMESPACE

InputMessage::InputMessage():
mMouseWheelDelta(0.0f)
{
}

bool InputMessage::IsKeyPressed(VKey key) const
{
	for (auto it : mPressedKeys)
	{
		if (it.mKey == key)
			return true;
	}

	return false;
}

bool InputMessage::IsKeyDown(VKey key) const
{
	for (auto it : mDownKeys)
	{
		if (it.mKey == key)
			return true;
	}

	return IsKeyPressed(key);
}

bool InputMessage::IsKeyReleased(VKey key) const
{
	for (auto it : mReleasedKeys)
	{
		if (it.mKey == key) 
			return true;
	}

	return false;
}

bool InputMessage::IsKeyRepeating(VKey key, float delay /*= 0.1f*/, float beginDelay /*= 0.5f*/) const
{
	float time = GetKeyPressingTime(key) - beginDelay;
	float repeatTime = Max(floor(time/delay)*delay, 0.0f);

	return time - AppTimer()->GetDeltaTime() < repeatTime && time > repeatTime;
}

float InputMessage::GetKeyPressingTime(VKey key) const
{
	for (auto it : mDownKeys)
	{
		if (it.mKey == key)
			return it.mPressedTime;
	}

	return 0;
}

Vec2F InputMessage::GetCursorPos(int id /*= 0*/) const
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

bool InputMessage::IsCursorPressed(int id /*= 0*/) const
{
	for (auto it : mCursors)
	{
		if (it.mId == id && it.mPressedTime < FLT_EPSILON)
			return true;
	}

	return false;
}

bool InputMessage::IsCursorDown(int id /*= 0*/) const
{
	for (auto it : mCursors)
	{
		if (it.mId == id)
			return true;
	}

	return false;
}

bool InputMessage::IsCursorReleased(int id /*= 0*/) const
{
	for (auto it : mReleasedCursors)
	{
		if (it.mId == id)
			return true;
	}

	return false;
}

float InputMessage::GetCursorPressingTime(int id /*= 0*/) const
{
	for (auto it : mCursors)
	{
		if (it.mId == id)
			return it.mPressedTime;
	}

	return 0;
}

Vec2F InputMessage::GetCursorDelta(int id /*= 0*/) const
{
	for (auto it : mCursors)
	{
		if (it.mId == id)
			return it.mDelta;
	}

	if (id == 0)
		return mMainCursorDelta;

	return Vec2F();
}

bool InputMessage::IsAltCursorPressed() const
{
	return IsKeyPressed((VKey)-1);
}

bool InputMessage::IsAltCursorDown() const
{
	return IsKeyDown((VKey)-1);
}

bool InputMessage::IsAltCursorReleased() const
{
	return IsKeyReleased((VKey)-1);
}

float InputMessage::GetAltCursorPressedTime() const
{
	return GetKeyPressingTime((VKey)-1);
}

bool InputMessage::IsAlt2CursorPressed() const
{
	return IsKeyPressed((VKey)-2);
}

bool InputMessage::IsAlt2CursorDown() const
{
	return IsKeyDown((VKey)-2);
}

bool InputMessage::IsAlt2CursorReleased() const
{
	return IsKeyReleased((VKey)-2);
}

float InputMessage::GetAlt2CursorPressedTime() const
{
	return GetKeyPressingTime((VKey)-2);
}

float InputMessage::GetMouseWheelDelta() const
{
	return mMouseWheelDelta;
}

InputMessage::CursorsArr const& InputMessage::GetCursors() const
{
	return mCursors;
}

InputMessage::KeysArr const& InputMessage::GetPressedKeys() const
{
	return mPressedKeys;
}

InputMessage::KeysArr const& InputMessage::GetDownKeys() const
{
	return mDownKeys;
}

InputMessage::KeysArr const& InputMessage::GetReleasedKeys() const
{
	return mReleasedKeys;
}


void InputMessage::KeyPressed(VKey key)
{
	if (IsKeyDown(key))
		return;

	mPressedKeys.Add(key);
}

void InputMessage::KeyReleased(VKey key)
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

int InputMessage::CursorPressed(const Vec2F& pos)
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

void InputMessage::CursorReleased(int id /*= 0*/)
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

void InputMessage::SetCursorPos(const Vec2F& pos, int id /*= 0*/)
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

void InputMessage::update(float dt)
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

void InputMessage::AltCursorPressed(const Vec2F& pos)
{
	KeyPressed(-1);
	SetCursorPos(pos);
}

void InputMessage::AltCursorReleased()
{
	KeyReleased(-1);
}

void InputMessage::Alt2CursorPressed(const Vec2F& pos)
{
	KeyPressed(-2);
	SetCursorPos(pos);
}

void InputMessage::Alt2CursorReleased()
{
	KeyReleased(-2);
}

void InputMessage::SetMouseWheelDelta(float delta)
{
	mMouseWheelDelta = delta;
}

CLOSE_O2_NAMESPACE