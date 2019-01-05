#include "ScriptObject.h"
#include "../Scene.h"
#include "../Application.h"

#include <regex>

namespace fse
{
	ScriptObject::ScriptObject(const std::string& scriptName) : FSEObject(), script_name_(scriptName)
	{
		
	}

	ScriptObject::ScriptObject(const std::string& scriptName, const sf::Vector2f& spawnPos) : FSEObject(spawnPos), script_name_(scriptName)
	{
	}

	void ScriptObject::spawned()
	{
		try {
			auto chai = scene_->getApplication()->getChai();
			chai->use(script_name_);

			std::smatch match;
			std::regex_search(script_name_, match, std::regex(R"((?!.*(?=\/|\\)).*(?=\.chai))"));

			const std::string classname = match.str();

			chai->set_global(chaiscript::var(std::ref(*scene_)), "scriptObjectScene");
			script_child_ = chai->eval(std::string(
				classname + "(scriptObjectScene.getObjectWithId("+  std::to_string(getID()) + "));"));

			initialized_ = true;

			if (spawnedFun != nullptr)
				spawnedFun(script_child_);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
			FSEObject::destroy();
		}
	}

	void ScriptObject::onDespawn()
	{
		try {
			if (onDespawnFun != nullptr)
				onDespawnFun(script_child_);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}

	void ScriptObject::update(float deltaTime)
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

	void ScriptObject::draw(sf::RenderTarget& target)
	{
		try
		{
			if (drawFun != nullptr)
				drawFun(script_child_, target);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}

	void ScriptObject::drawNormals(sf::RenderTarget& target)
	{
		try {
			if (drawNormalsFun != nullptr)
				drawNormalsFun(script_child_, target);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}

	void ScriptObject::drawSpecular(sf::RenderTarget& target)
	{
		try {
			if (drawSpecularFun != nullptr)
				drawSpecularFun(script_child_, target);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}

	void ScriptObject::setPosition(const sf::Vector2f position)
	{
		try {
			if (setPositionFun != nullptr)
				setPositionFun(script_child_, position);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}

	sf::Vector2f ScriptObject::getPosition()
	{
		try {
			if (getPositionFun != nullptr)
				return getPositionFun(script_child_);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
		return FSEObject::getPosition();
	}

	sf::FloatRect ScriptObject::GetAABBs() const
	{
		try {
			if (getAABBsFun != nullptr)
				return getAABBsFun(script_child_);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
		return FSEObject::GetAABBs();
	}

	void ScriptObject::BeginContact(FSEObject* otherObject, b2Contact* contact)
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

	void ScriptObject::EndContact(FSEObject* otherObject, b2Contact* contact)
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

	void ScriptObject::PreSolve(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold)
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

	void ScriptObject::PostSolve(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
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

	bool ScriptObject::destroy()
	{
		try {
			if (destroyFun != nullptr)
				return destroyFun(script_child_);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
		return FSEObject::destroy();
	}

	void ScriptObject::BeginContactComponents(FSEObject* otherObject, b2Contact* contact)
	{
		try {
			if (beginContactComponentsFun != nullptr)
				beginContactComponentsFun(script_child_, otherObject, contact);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}

	void ScriptObject::EndContactComponents(FSEObject* otherObject, b2Contact* contact)
	{
		try {
			if (endContactComponentsFun != nullptr)
				endContactComponentsFun(script_child_, otherObject, contact);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}

	void ScriptObject::PreSolveComponents(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold)
	{
		try {
		if (preSolveComponentsFun != nullptr)
			preSolveComponentsFun(script_child_, otherObject, contact, oldManifold);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}

	void ScriptObject::PostSolveComponents(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
	{
		try {
		if (postSolveComponentsFun != nullptr)
			postSolveComponentsFun(script_child_, otherObject, contact, impulse);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	}


	FSE_CHAI_REGISTER(ScriptObject)
	{
		RegisterChaiUserTypeFromRTTR<ScriptObject>(chai);
		chai.add(chaiscript::base_class<fse::FSEObject, ScriptObject>());
		chai.add(chaiscript::constructor<ScriptObject(const std::string&)>(), "ScriptObject");
		chai.add(chaiscript::fun([](const ScriptObject* object)
		{
			return object->initialized_;
		}), "isInitialized");

		chai.add(chaiscript::fun([](const ScriptObject* object)
		{
			return object->script_name_;
		}), "getScriptName");

		chai.add(chaiscript::fun([](const ScriptObject* object)
		{
			return object->script_child_;
		}), "_child");

		chai.add(chaiscript::fun(&ScriptObject::spawnedFun), "spawnedCallback");
		chai.add(chaiscript::fun(&ScriptObject::onDespawnFun), "onDespawnCallback");

		chai.add(chaiscript::fun(&ScriptObject::updateFun), "updateCallback");
		chai.add(chaiscript::fun(&ScriptObject::drawFun), "drawCallback");
		chai.add(chaiscript::fun(&ScriptObject::drawNormalsFun), "drawNormalsCallback");
		chai.add(chaiscript::fun(&ScriptObject::drawSpecularFun), "drawSpecularCallback");

		chai.add(chaiscript::fun(&ScriptObject::setPositionFun), "setPositionCallback");
		chai.add(chaiscript::fun(&ScriptObject::getPositionFun), "getPositionCallback");
		chai.add(chaiscript::fun(&ScriptObject::getAABBsFun), "getAABBsCallback");

		chai.add(chaiscript::fun(&ScriptObject::beginContactFun), "beginContactCallback");
		chai.add(chaiscript::fun(&ScriptObject::endContactFun), "endContactCallback");

		chai.add(chaiscript::fun(&ScriptObject::preSolveFun), "preSolveCallback");
		chai.add(chaiscript::fun(&ScriptObject::postSolveFun), "postSolveCallback");

		chai.add(chaiscript::fun(&ScriptObject::destroyFun), "destroyCallback");

		chai.add(chaiscript::fun(&ScriptObject::beginContactComponentsFun), "beginContactComponentsCallback");
		chai.add(chaiscript::fun(&ScriptObject::endContactComponentsFun), "endContactComponentsCallback");

		chai.add(chaiscript::fun(&ScriptObject::preSolveComponentsFun), "preSolveComponentsCallback");
		chai.add(chaiscript::fun(&ScriptObject::postSolveComponentsFun), "postSolveComponentsCallback");

	}

}

#include <rttr/registration>
RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace fse;
	registration::class_<ScriptObject>("fse::ScriptObject")
		.constructor<const std::string& >()
		(
			policy::ctor::as_std_shared_ptr,
			parameter_names("script name")
			)
		.constructor<const std::string&, const sf::Vector2f&>()
		(
			policy::ctor::as_std_shared_ptr,
			parameter_names("script name", "spawn position")
			)
		.property_readonly("script_name_", &ScriptObject::script_name_)
		.property_readonly("initialized_", &ScriptObject::initialized_)
		.property_readonly("script_child_", &ScriptObject::script_child_)
		;
}
