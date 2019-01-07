#include "Component.h"
#include "../FSEObject/FSEObject.h"
#include <Box2D/Box2D.h>
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
		return object_ != nullptr;
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

	FSE_CHAI_REGISTER(Component)
	{
		RegisterChaiUserTypeFromRTTR<Component>(chai);
		chai.add(chaiscript::fun(static_cast<FSEObject* (Component::*)() const>(&Component::getAttachedObject)), "getAttachedObject");
		chai.add(chaiscript::fun(static_cast<bool (Component::*)() const>(&Component::isAttached)), "isAttached");



		chai.add(chaiscript::user_type<std::weak_ptr<Component>>(), "WeakComponent");
		chai.add(chaiscript::type_conversion<std::weak_ptr<Component>, std::shared_ptr<Component>>([](const std::weak_ptr<Component>& t_bt) { return t_bt.lock(); }));
		chai.add(chaiscript::fun([](const std::weak_ptr<Component> & weak_obj)
		{
			return weak_obj.lock();
		}), "lock");

		chai.add(chaiscript::fun([](const std::weak_ptr<Component> & weak_obj)
		{
			return !weak_obj.expired();
		}), "valid");



		chai.add(chaiscript::vector_conversion<std::vector<std::shared_ptr<Component>>>());
		chai.add(chaiscript::bootstrap::standard_library::vector_type<std::vector<std::shared_ptr<Component>>>("ComponentList"));


		chai.add(chaiscript::vector_conversion<std::vector<std::weak_ptr<Component>>>());
		chai.add(chaiscript::bootstrap::standard_library::vector_type<std::vector<std::weak_ptr<Component>>>("WeakComponentList"));
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