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
}


RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace fse;

	registration::class_<Component>("fse::Component")
;
}