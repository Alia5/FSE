#include "Component.h"

#include <rttr/registration.h>

namespace fse
{
	Component::Component()
	{
	}

	void Component::onAttach()
	{
	}

	void Component::onDetach()
	{
	}

	FSEObject* Component::getAttachedObject() const
	{
		return object_;
	}

	void Component::attachToObject(FSEObject* object)
	{
		object_ = object;
		onAttach();
	}

	bool Component::isAttached() const
	{
		if (object_ == nullptr)
			return false;
		return true;
	}

	void Component::detach()
	{
		onDetach();
		object_ = nullptr;
	}

	void Component::BeginContact(FSEObject* otherObject, b2Contact* contact)
	{
	}

	void Component::EndContact(FSEObject* otherObject, b2Contact* contact)
	{
	}

	void Component::PreSolve(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold)
	{
	}

	void Component::PostSolve(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
	{
	}
}


RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace fse;

	registration::class_<Component>("fse::Component")
	.property_readonly("attached_", &Component::isAttached)
;
}