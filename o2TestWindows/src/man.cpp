#include "man.h"

#include "render_system/sprite.h"
#include "app/application.h"

#include "man_field.h"

OPEN_O2_NAMESPACE

cMan::WalkAnimation::WalkAnimation( grSprite* animatingSprite, pugi::xml_node& xmlNode )
{
	mAnimatingSprite = animatingSprite;
	mAnimating = false;

	mCurrTime = 0;

	mFramesDelay = xmlNode.child("framesDelay").attribute("v").as_float();

	if (pugi::xml_node framesNode = xmlNode.child("frames"))
	{
		for (pugi::xml_node obj = framesNode.child("frame"); obj; obj = obj.next_sibling("frame"))
		{
			mFrames.push_back(cXmlTools::node2rectf(obj));
		}
	}

	if (pugi::xml_node animationsNode = xmlNode.child("animations"))
	{
		for (pugi::xml_node obj = animationsNode.child("anim"); obj; obj = obj.next_sibling("anim"))
		{
			std::string id = obj.attribute("id").value();
			int beg = obj.attribute("begin").as_int();
			int end = obj.attribute("end").as_int();
			
			mAnimations[id].x = beg;
			mAnimations[id].y = end;
		}
	}

	mCurrAnim[0] = 0; mCurrAnim[1] = 1;
}

void cMan::WalkAnimation::update( float dt )
{
	mCurrTime += dt;
	int frameId = (int)(mCurrTime/mFramesDelay)%(mCurrAnim[1] - mCurrAnim[0]) + mCurrAnim[0];

	if (!mAnimating)
		frameId = mCurrAnim[0];

	mAnimatingSprite->setTextureSrcRect(mFrames[frameId]);
}

void cMan::WalkAnimation::setPlayingAnim( const std::string& id )
{
	mCurrAnim[0] = mAnimations[id].x;
	mCurrAnim[1] = mAnimations[id].y;
}


cMan::cMan( cManField* manField, pugi::xml_node& xmlNode )
{
	mManField = manField;
	
	mSpriteOffset = cXmlTools::node2vecf(xmlNode.child("spriteOffset"));
	mSprite = mnew grSprite(mManField->mApplication->getRenderSystem(), xmlNode.child("sprite"));

	mWalkAnim = mnew WalkAnimation(mSprite, xmlNode);
	mWalkPath = mnew WalkPath(this);
}

cMan::~cMan()
{
	safe_release(mWalkAnim);
	safe_release(mSprite);
	safe_release(mWalkPath);
}

void cMan::draw()
{
	mSprite->draw();
}

void cMan::update( float dt )
{
	mSprite->setPosition( mPosition.scale(mManField->mClusterSize) + mSpriteOffset + mManField->mClusterSize*0.5f );

	vec2f lastPos = mPosition;
	mWalkPath->update(dt);

	vec2f diff = mPosition - lastPos;

	if (abs(diff.x) > abs(diff.y))
	{
		if (diff.x < 0)
			mWalkAnim->setPlayingAnim("walk_left");
		else
			mWalkAnim->setPlayingAnim("walk_right");
	}
	else
	{		
		if (diff.y < 0)
			mWalkAnim->setPlayingAnim("walk_up");
		else
			mWalkAnim->setPlayingAnim("walk_down");
	}

	mWalkAnim->mAnimating = mWalkPath->mMoving;
	mWalkAnim->update(dt);
}

void cMan::setWayPoint( const vec2f& point )
{
	mWalkPath->setupWaypoint(point);
}


void cMan::WalkPath::setupWaypoint( const vec2f& point )
{
	mWaypoints.clear();

	int begin = mMan->mManField->mWaypointWeb->getNearestWaypoint(mMan->mPosition);
	int end = mMan->mManField->mWaypointWeb->getNearestWaypoint(point - vec2f(0.5f, 0.5f));

	dijkstraPathSearch(*mMan->mManField->mWaypointWeb, mWaypoints, begin, end);

	mCurrentWaypoint = 0;
	mMovingTime = 0;
	mMoving = true;
}

void cMan::WalkPath::update( float dt )
{
	if (!mMoving)
		return;

	mMovingTime += dt;

	while (mCurrentWaypoint < (int)mWaypoints.size() - 1 && 
		   mMovingTime > mWaypoints[mCurrentWaypoint + 1].mG)
	{
		mMovingTime -= mWaypoints[mCurrentWaypoint + 1].mG;
		mCurrentWaypoint++;
	}

	if (mCurrentWaypoint == mWaypoints.size() - 1)
	{
		mMan->mPosition = mWaypoints[mCurrentWaypoint].mPoint;
		mMoving = false;
	}
	else
	{
		float coef = mMovingTime/mWaypoints[mCurrentWaypoint + 1].mG;
		
		vec2f p1 = mWaypoints[mCurrentWaypoint].mPoint;
		vec2f p2 = mWaypoints[mCurrentWaypoint + 1].mPoint;
		mMan->mPosition = (p2 - p1)*coef + p1;
	}
}

cMan::WalkPath::WalkPath( cMan* man ):
	mMan(man), mCurrentWaypoint(0), mMoving(false)
{
}

CLOSE_O2_NAMESPACE