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
		FSEObject * objectA = static_cast<FSEObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
		FSEObject * objectB = static_cast<FSEObject*>(contact->GetFixtureB()->GetBody()->GetUserData());

		if (objectA != nullptr && objectB != nullptr)
		{
			objectA->BeginContact(objectB, contact);
			objectB->BeginContact(objectA, contact);
		}

	}

	void PhysContactListener::EndContact(b2Contact* contact)
	{
		FSEObject * objectA = static_cast<FSEObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
		FSEObject * objectB = static_cast<FSEObject*>(contact->GetFixtureB()->GetBody()->GetUserData());

		if (objectA != nullptr && objectB != nullptr)
		{
			objectA->EndContact(objectB, contact);
			objectB->EndContact(objectA, contact);
		}

	}

	void PhysContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		FSEObject * objectA = static_cast<FSEObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
		FSEObject * objectB = static_cast<FSEObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
		
		if (objectA != nullptr && objectB != nullptr)
		{
			objectA->PreSolve(objectB, contact, oldManifold);
			objectB->PreSolve(objectA, contact, oldManifold);
		}

	}

	void PhysContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		FSEObject * objectA = static_cast<FSEObject*>(contact->GetFixtureA()->GetBody()->GetUserData());
		FSEObject * objectB = static_cast<FSEObject*>(contact->GetFixtureB()->GetBody()->GetUserData());
		
		if (objectA != nullptr && objectB != nullptr)
		{
			objectA->PostSolve(objectB, contact, impulse);
			objectB->PostSolve(objectA, contact, impulse);
		}

	}
}
