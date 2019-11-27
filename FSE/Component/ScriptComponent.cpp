#include "ScriptComponent.h"
#include "../FSEObject/FSEObject.h"
#include "../Application.h"

#include <Box2D/Box2D.h>
#include <rttr/registration.h>

namespace fse
{
	ScriptComponent::ScriptComponent(const std::string& script_name) : Component(), script_name_(script_name)
	{
	}


	void ScriptComponent::update(float deltaTime)
	{
		try {
			if (updateFun != nullptr)
				updateFun(script_child_, deltaTime);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}

	void ScriptComponent::onAttach()
	{
		try {
			auto chai = getAttachedObject()->getScene()->getApplication()->getChai();
			chai->use(script_name_);

			std::smatch match;
			std::regex_search(script_name_, match, std::regex(R"((?!.*(?=\/|\\)).*(?=\.chai))"));

			const std::string classname = match.str();

			chai->set_global(chaiscript::var(std::ref(*getAttachedObject())), "scriptComponentObject");
			script_child_ = chai->eval(std::string(
				classname + "(scriptComponentObject.getComponents().back().lock());"));

			initialized_ = true;

			if (onAttachFun != nullptr)
				onAttachFun(script_child_);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
			getAttachedObject()->detachComponent(this);
		}
	}

	void ScriptComponent::onDetach()
	{	
		try {
			if (onDetachFun != nullptr)
				onDetachFun(script_child_);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}

	void ScriptComponent::BeginContact(FSEObject* otherObject, b2Contact* contact)
	{
		try {
			if (beginContactFun != nullptr)
				beginContactFun(script_child_, otherObject, contact);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}

	void ScriptComponent::EndContact(FSEObject* otherObject, b2Contact* contact)
	{
		try {
			if (endContactFun != nullptr)
				endContactFun(script_child_, otherObject, contact);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}

	void ScriptComponent::PreSolve(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold)
	{
		try
		{
			if (preSolveFun != nullptr)
				preSolveFun(script_child_, otherObject, contact, oldManifold);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}

	void ScriptComponent::PostSolve(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
	{
		try {
			if (postSolveFun != nullptr)
				postSolveFun(script_child_, otherObject, contact, impulse);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}


	FSE_CHAI_REGISTER(ScriptComponent)
	{
		RegisterChaiUserTypeFromRTTR<ScriptComponent>(chai);
		chai.add(chaiscript::base_class<fse::Component, ScriptComponent>());
		chai.add(chaiscript::constructor<ScriptComponent(const std::string&)>(), "ScriptComponent");
		chai.add(chaiscript::fun([](const ScriptComponent* object)
		{
			return object->initialized_;
		}), "isInitialized");

		chai.add(chaiscript::fun([](const ScriptComponent* object)
		{
			return object->script_name_;
		}), "getScriptName");

		chai.add(chaiscript::fun([](const ScriptComponent* object)
		{
			return object->script_child_;
		}), "_child");



		chai.add(chaiscript::fun(&ScriptComponent::updateFun), "updateCallback");
		chai.add(chaiscript::fun(&ScriptComponent::onAttachFun), "onAttachCallback");
		chai.add(chaiscript::fun(&ScriptComponent::onDetachFun), "onDetachCallback");

		chai.add(chaiscript::fun(&ScriptComponent::beginContactFun), "beginContactCallback");
		chai.add(chaiscript::fun(&ScriptComponent::endContactFun), "endContactCallback");

		chai.add(chaiscript::fun(&ScriptComponent::preSolveFun), "preSolveCallback");
		chai.add(chaiscript::fun(&ScriptComponent::postSolveFun), "postSolveCallback");
	}
}


RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace fse;

	registration::class_<ScriptComponent>("fse::ScriptComponent")
		.constructor<const std::string& >()
		(
			policy::ctor::as_std_shared_ptr,
			parameter_names("script name")
			)
		.property_readonly("script_name_", &ScriptComponent::script_name_)
		(
			metadata("CTOR_ARG", "script name")
		)
		.property_readonly("initialized_", &ScriptComponent::initialized_)
		.property_readonly("script_child_", &ScriptComponent::script_child_)
;
}
