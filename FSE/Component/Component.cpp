#include "Component.h"

#include <rttr/registration.h>

namespace fse
{
	Component::Component()
	{
	}

	FSEObject* Component::getAttachedObject() const
	{
		return object_;
	}

	void Component::attachToObject(FSEObject* object)
	{
		object_ = object;
	}

	bool Component::isAttached() const
	{
		if (object_ == nullptr)
			return false;
		return true;
	}

	void Component::detach()
	{
		object_ = nullptr;
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