#include "PhysContactListener.h"
#include "FSEObject/FSEObject.h"
namespace fse
{
	PhysContactListener::PhysContactListener() {
    
	}

	PhysContactListener::~PhysContactListener()
	{
	}

	void PhysContactListener::BeginContact(b2Contact* contact)
	{
		auto rawObjectA = reinterpret_cast<FSEObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
		auto rawObjectB = reinterpret_cast<FSEObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
		if (rawObjectA != nullptr && rawObjectB != nullptr)
		{
			const auto objectA = rawObjectA->weak_from_this();
			const auto objectB = rawObjectB->weak_from_this();
			if (!objectA.expired() && !objectB.expired())
			{
				objectA.lock()->BeginContact(objectB, contact);
				objectB.lock()->BeginContact(objectA, contact);
			}
		}
	}

	void PhysContactListener::EndContact(b2Contact* contact)
	{
		auto rawObjectA = reinterpret_cast<FSEObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
		auto rawObjectB = reinterpret_cast<FSEObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
		if (rawObjectA != nullptr && rawObjectB != nullptr)
		{
			const auto objectA = rawObjectA->weak_from_this();
			const auto objectB = rawObjectB->weak_from_this();
			if (!objectA.expired() && !objectB.expired())
			{
				objectA.lock()->EndContact(objectB, contact);
				objectB.lock()->EndContact(objectA, contact);
			}
		}
	}

	void PhysContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		auto rawObjectA = reinterpret_cast<FSEObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
		auto rawObjectB = reinterpret_cast<FSEObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
		if (rawObjectA != nullptr && rawObjectB != nullptr)
		{
			const auto objectA = rawObjectA->weak_from_this();
			const auto objectB = rawObjectB->weak_from_this();
			if (!objectA.expired() && !objectB.expired())
			{
				objectA.lock()->PreSolve(objectB, contact, oldManifold);
				objectB.lock()->PreSolve(objectA, contact, oldManifold);
			}
		}
	}

	void PhysContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		auto rawObjectA = reinterpret_cast<FSEObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
		auto rawObjectB = reinterpret_cast<FSEObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
		if (rawObjectA != nullptr && rawObjectB != nullptr)
		{
			const auto objectA = rawObjectA->weak_from_this();
			const auto objectB = rawObjectB->weak_from_this();
			if (!objectA.expired() && !objectB.expired())
			{
				objectA.lock()->PostSolve(objectB, contact, impulse);
				objectB.lock()->PostSolve(objectA, contact, impulse);
			}
		}

	}
}
