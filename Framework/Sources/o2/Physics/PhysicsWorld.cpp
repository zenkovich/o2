#include "o2/stdafx.h"
#include "PhysicsWorld.h"

#include "Box2D/Dynamics/b2Body.h"
#include "o2/Scene/Physics/RigidBody.h"

namespace o2
{
	DECLARE_SINGLETON(PhysicsWorld);

	PhysicsWorld::PhysicsWorld():
		mWorld(gravity)
	{
		auto debugDraw = mnew PhysicsDebugDraw();
		mWorld.SetDebugDraw(debugDraw);
		debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_aabbBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit | b2Draw::e_jointBit);
	}

	void PhysicsWorld::PreUpdate()
	{
		mWorld.SetGravity(gravity);

		for (b2Body* body = mWorld.GetBodyList(); body; body = body->GetNext())
		{
			auto rigidBody = (RigidBody*)body->GetUserData();
			auto transform = rigidBody->transform;

			body->SetTransform(transform->GetWorldPosition(), transform->GetWorldAngle());
		}
	}

	void PhysicsWorld::Update(float dt)
	{
		mWorld.Step(dt, velocityIterations, positionIterations);
	}

	void PhysicsWorld::PostUpdate()
	{
		for (b2Body* body = mWorld.GetBodyList(); body; body = body->GetNext())
		{
			auto rigidBody = (RigidBody*)body->GetUserData();
			auto transform = rigidBody->transform;

			transform->SetWorldPosition(body->GetPosition());
			transform->SetWorldAngle(body->GetAngle());
		}
	}

	void PhysicsWorld::DrawDebug()
	{
		mWorld.DrawDebugData();
	}

	void PhysicsDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		o2Render.DrawAACircle(center, radius, color);
	}

	void PhysicsDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		Vector<Vec2F> points; 
		points.Resize(vertexCount + 1);
		for (int i = 0; i < vertexCount; i++)
			points[i] = vertices[i];

		points.Last() = vertices[0];

		o2Render.DrawAALine(points, color);
	}

	void PhysicsDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		o2Render.DrawAALine(p1, p2, color);
	}

	void PhysicsDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		o2Render.DrawFilledCircle(center, radius, color);
	}

	void PhysicsDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		Vector<Vec2F> points;
		points.Resize(vertexCount);
		for (int i = 0; i < vertexCount; i++)
			points[i] = vertices[i];

		o2Render.DrawFilledPolygon(points, color);
	}

	void PhysicsDebugDraw::DrawTransform(const b2Transform& xf)
	{
		o2Render.DrawBasis(Basis(xf.p, xf.q.GetXAxis(), xf.q.GetYAxis()));
	}

}
