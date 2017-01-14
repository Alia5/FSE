#pragma once

#include <Box2D/Box2D.h>

namespace fse
{
	class GameObject;
	class PhysContactListener : public b2ContactListener {

	public:
		PhysContactListener();
		~PhysContactListener();

		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

	private:

	};

}