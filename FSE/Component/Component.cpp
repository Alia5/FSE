#include "Component.h"
#include "../FSEObject/FSEObject.h"
#include <Box2D/Box2D.h>
#include <rttr/registration.h>

namespace fse
{
	Component::Component()
	{
	}

	void Component::update(float deltaTime)
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

	FSE_V8_REGISTER(Component)
	{
		v8::HandleScope handle_scope(isolate);
		v8pp::class_<Component, v8pp::shared_ptr_traits> Component_class(isolate);
		Component_class.auto_wrap_objects(true);
		Component_class.function("update", &Component::update);
		Component_class.function("onAttach", &Component::onAttach);
		Component_class.function("onDetach", &Component::onDetach);
		Component_class.function("getAttachedObject", [](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				v8::Isolate* isolate = args.GetIsolate();
				const auto This = v8pp::from_v8<Component*>(isolate, args.This());
				typedef v8pp::class_<fse::FSEObject> my_class_wrapper;
				return my_class_wrapper::import_external(isolate, This->object_);
			});
		//Component_class.function("attachToObject", &Component::attachToObject);
		Component_class.function("isAttached", &Component::isAttached);
		Component_class.function("detach", &Component::detach);
		//Component_class.function("BeginContact", &Component::BeginContact);
		//Component_class.function("EndContact", &Component::EndContact);
		//Component_class.function("PreSolve", &Component::PreSolve);
		//Component_class.function("PostSolve", &Component::PostSolve);
		module.class_("Component", Component_class);

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