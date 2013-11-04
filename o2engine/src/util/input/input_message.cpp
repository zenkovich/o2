#include "input_message.h"

#include <algorithm>

#include "util/math/math.h"

OPEN_O2_NAMESPACE

bool cInputMessage::isKeyPressed( VKey key ) const
{
	for (KeysVec::const_iterator it = mPressedKeys.cbegin(); it != mPressedKeys.cend(); it++)
		if (it->mKey == key) return true;

	return false;
}

bool cInputMessage::isKeyDown( VKey key ) const
{
	for (KeysVec::const_iterator it = mDownKeys.cbegin(); it != mDownKeys.cend(); it++)
		if (it->mKey == key) return true;

	return false;
}

bool cInputMessage::isKeyReleased( VKey key ) const
{
	for (KeysVec::const_iterator it = mReleasedKeys.cbegin(); it != mReleasedKeys.cend(); it++)
		if (it->mKey == key) return true;

	return false;
}

const vec2f& cInputMessage::getCursorPos( int idx /*= 0*/ ) const
{
	if (idx < 0 || idx > (int)(mCursorsPositions.size() - 1))
		return vec2f();

	return mCursorsPositions[idx];
}

void cInputMessage::keyPressed( VKey key )
{
	if (isKeyDown(key))
		return;

	mPressedKeys.push_back(key);
}

void cInputMessage::keyReleased( VKey key )
{
	for (KeysVec::const_iterator it = mDownKeys.cbegin(); it != mDownKeys.cend(); it++)
	{
		if (it->mKey == key)
		{
			mDownKeys.erase(it);
			break;
		}
	}
	mReleasedKeys.push_back(key);
}

void cInputMessage::setCursorPos( const vec2f& pos, int idx /*= 0*/ )
{
	if (idx > (int)mCursorsPositions.size() - 1)
	{
		for (int i = 0; i < idx + 1; i++)
			mCursorsPositions.push_back(vec2f());
	}

	mCursorsPositions[0] = pos;
}

void cInputMessage::update(float dt)
{
	mDownKeys.insert(mDownKeys.end(), mPressedKeys.begin(), mPressedKeys.end());
	mPressedKeys.clear();
	mReleasedKeys.clear();

	for (KeysVec::iterator it = mDownKeys.begin(); it != mDownKeys.end(); it++)
	{
		(*it).mPressedTime += dt;
	}
}

CLOSE_O2_NAMESPACE