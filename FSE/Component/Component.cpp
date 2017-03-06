#include "Component.h"

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
