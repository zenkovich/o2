#include "o2/stdafx.h"
#include "ICollider.h"

#include "o2/Config/ProjectConfig.h"
#include "o2/Physics/PhysicsWorld.h"
#include "o2/Scene/Physics/RigidBody.h"
#include "o2/Scene/Scene.h"

namespace o2
{

    ICollider::ICollider()
    {}

    ICollider::ICollider(const ICollider& other):
        Component(other), mFriction(other.mFriction), mDensity(other.mDensity), mRestitution(other.mRestitution), 
        mLayer(other.mLayer), mIsSensor(other.mIsSensor)
    {}

    ICollider::~ICollider()
    {
        RemoveFromRigidBody();
    }

    ICollider& ICollider::operator=(const ICollider& other)
    {
        Component::operator=(other);

        mFriction = other.mFriction;
        mDensity = other.mDensity;
        mRestitution = other.mRestitution;
        mLayer = other.mLayer;
        mIsSensor = other.mIsSensor;

        OnShapeChanged();

        return *this;
    }

    void ICollider::SetFriction(float value)
    {
        mFriction = value;

        if (mFixture)
            mFixture->SetFriction(mFriction);
    }

    float ICollider::GetFriction() const
    {
        return mFriction;
    }

    void ICollider::SetDensity(float value)
    {
        mDensity = value;

        if (mFixture)
            mFixture->SetDensity(mDensity);
    }

    float ICollider::GetDensity() const
    {
        return mDensity;
    }

    void ICollider::SetRestitution(float value)
    {
        mRestitution = value;

        if (mFixture)
            mFixture->SetRestitution(mRestitution);
    }

    float ICollider::GetRestitution() const
    {
        return mRestitution;
    }

    void ICollider::SetLayer(const String& layer)
    {
        mLayer = layer;
    }

    const String& ICollider::GetLayer() const
    {
        return mLayer;
    }

    void ICollider::SetIsSensor(bool value)
    {
        mIsSensor = value;

        if (mFixture)
            mFixture->SetSensor(mIsSensor);
    }

    bool ICollider::IsSensor() const
    {
        return mIsSensor;
    }

    bool ICollider::IsAvailableFromCreateMenu()
    {
        return false;
    }

    void ICollider::AddToRigidBody(RigidBody* body)
    {
        auto thisTransform = mOwner.Lock()->transform;
        auto bodyTransform = body->transform;
        Basis thisBasis = thisTransform->GetWorldNonSizedBasis(); 
        Basis bodyBasis = bodyTransform->GetWorldNonSizedBasis(); 
        bodyBasis.xv.Normalize(); bodyBasis.yv.Normalize();
        Basis relativeTransform = thisBasis*(bodyBasis.Inverted());

        float invScale = 1.0f/o2Config.physics.scale;
        relativeTransform.origin *= invScale;
        relativeTransform.xv *= invScale;
        relativeTransform.yv *= invScale;

        b2FixtureDef fixture;
        fixture.shape = GetShape(relativeTransform);
        fixture.density = mDensity;
        fixture.friction = mFriction;
        fixture.restitution = mRestitution;
        //fixture.filter.groupIndex = (int16)World::Instance().settings.GetLayerId(_layer);
        fixture.userData = this;
        fixture.isSensor = mIsSensor;

        if (!fixture.shape) {
            return;
        }

        mFixture = body->mBody->CreateFixture(&fixture);
        mRigidBodyComp = body;
    }

    void ICollider::RemoveFromRigidBody()
    {
        if (mRigidBodyComp && mRigidBodyComp->mBody) {
            mRigidBodyComp->mBody->DestroyFixture(mFixture);
        }

        mRigidBodyComp = nullptr;
        mFixture = nullptr;
    }

    Ref<RigidBody> ICollider::FindRigidBody() const
    {
        auto itActor = mOwner.Lock();
        while (itActor)
        {
            if (auto body = DynamicCast<RigidBody>(itActor))
                return body;

            itActor = itActor->GetParent().Lock();
        }

        return nullptr;
    }

    void ICollider::OnShapeChanged()
    {
        if (auto rigidBody = FindRigidBody()) {
            rigidBody->RemoveCollider(this);
            rigidBody->AddCollider(this);
        }
    }

    b2Shape* ICollider::GetShape(const Basis& transform)
    {
        return nullptr;
    }

    void ICollider::OnTransformUpdated()
    {
#if IS_EDITOR
        if (!o2Scene.IsEditorPlaying())
            OnShapeChanged();
#endif
    }

    void ICollider::OnAddToScene()
    {
        if (auto rigidBody = FindRigidBody())
            rigidBody->AddCollider(this);

        Component::OnAddToScene();
    }

    void ICollider::OnRemoveFromScene()
    {
        RemoveFromRigidBody();
        Component::OnRemoveFromScene();
    }

}

DECLARE_TEMPLATE_CLASS(o2::LinkRef<o2::ICollider>);
// --- META ---

DECLARE_CLASS(o2::ICollider, o2__ICollider);
// --- END META ---
