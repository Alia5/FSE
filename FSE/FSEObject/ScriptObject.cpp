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

		auto chai = scene_->getApplication()->getChai();
		chai->use(script_name_);

		std::smatch match;
		std::regex_search(script_name_, match, std::regex(R"((?!.*(?=\/|\\)).*(?=\.chai))"));

		const std::string classname = match.str();
		
		script_child_ = chai->eval(std::string(
			classname + "(gameScene.getObjectWithId("+  std::to_string(getID()) + "));"));

		initialized_ = true;

		if (spawnedFun != nullptr)
			spawnedFun(script_child_);
	}

	void ScriptObject::onDespawn()
	{
		if (onDespawnFun != nullptr)
			onDespawnFun(script_child_);
	}

	void ScriptObject::update(float deltaTime)
	{
		if (updateFun != nullptr)
			updateFun(script_child_, deltaTime);
	}

	void ScriptObject::draw(sf::RenderTarget& target)
	{
		if (drawFun != nullptr)
			drawFun(script_child_, target);
	}

	void ScriptObject::drawNormals(sf::RenderTarget& target)
	{
		if (drawNormalsFun != nullptr)
			drawNormalsFun(script_child_, target);
	}

	void ScriptObject::drawSpecular(sf::RenderTarget& target)
	{
		if (drawSpecularFun != nullptr)
			drawSpecularFun(script_child_, target);
	}

	void ScriptObject::setPosition(const sf::Vector2f position)
	{
		if (setPositionFun != nullptr)
			setPositionFun(script_child_, position);
	}

	sf::Vector2f ScriptObject::getPosition()
	{
		if (getPositionFun != nullptr)
			return getPositionFun(script_child_);
		return FSEObject::getPosition();
	}

	sf::FloatRect ScriptObject::GetAABBs() const
	{
		if (getAABBsFun != nullptr)
			return getAABBsFun(script_child_);
		return FSEObject::GetAABBs();
	}

	void ScriptObject::BeginContact(FSEObject* otherObject, b2Contact* contact)
	{
		if (beginContactFun != nullptr)
			beginContactFun(script_child_, otherObject, contact);
	}

	void ScriptObject::EndContact(FSEObject* otherObject, b2Contact* contact)
	{
		if (endContactFun != nullptr)
			endContactFun(script_child_, otherObject, contact);
	}

	void ScriptObject::PreSolve(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold)
	{
		if (preSolveFun != nullptr)
			preSolveFun(script_child_, otherObject, contact, oldManifold);
	}

	void ScriptObject::PostSolve(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
	{
		if (postSolveFun != nullptr)
			postSolveFun(script_child_, otherObject, contact, impulse);
	}

	bool ScriptObject::destroy()
	{
		if (destroyFun != nullptr)
			return destroyFun(script_child_);
		return FSEObject::destroy();
	}

	void ScriptObject::BeginContactComponents(FSEObject* otherObject, b2Contact* contact)
	{
		if (beginContactComponentsFun != nullptr)
			beginContactComponentsFun(script_child_, otherObject, contact);
	}

	void ScriptObject::EndContactComponents(FSEObject* otherObject, b2Contact* contact)
	{
		if (endContactComponentsFun != nullptr)
			endContactComponentsFun(script_child_, otherObject, contact);
	}

	void ScriptObject::PreSolveComponents(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold)
	{
		if (preSolveComponentsFun != nullptr)
			preSolveComponentsFun(script_child_, otherObject, contact, oldManifold);
	}

	void ScriptObject::PostSolveComponents(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
	{
		if (postSolveComponentsFun != nullptr)
			postSolveComponentsFun(script_child_, otherObject, contact, impulse);
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
			return object->script_child_;
		}), "_child");

		chai.add(chaiscript::fun(&ScriptObject::spawnedFun), "spawnedFun");
		chai.add(chaiscript::fun(&ScriptObject::onDespawnFun), "onDespawnFun");

		chai.add(chaiscript::fun(&ScriptObject::updateFun), "updateFun");
		chai.add(chaiscript::fun(&ScriptObject::drawFun), "drawFun");
		chai.add(chaiscript::fun(&ScriptObject::drawNormalsFun), "drawNormalsFun");
		chai.add(chaiscript::fun(&ScriptObject::drawSpecularFun), "drawSpecularFun");

		chai.add(chaiscript::fun(&ScriptObject::setPositionFun), "setPositionFun");
		chai.add(chaiscript::fun(&ScriptObject::getPositionFun), "getPositionFun");
		chai.add(chaiscript::fun(&ScriptObject::getAABBsFun), "getAABBsFun");

		chai.add(chaiscript::fun(&ScriptObject::beginContactFun), "beginContactFun");
		chai.add(chaiscript::fun(&ScriptObject::endContactFun), "endContactFun");

		chai.add(chaiscript::fun(&ScriptObject::preSolveFun), "preSolveFun");
		chai.add(chaiscript::fun(&ScriptObject::postSolveFun), "postSolveFun");

		chai.add(chaiscript::fun(&ScriptObject::destroyFun), "destroyFun");

		chai.add(chaiscript::fun(&ScriptObject::beginContactComponentsFun), "beginContactComponentsFun");
		chai.add(chaiscript::fun(&ScriptObject::endContactComponentsFun), "endContactComponentsFun");

		chai.add(chaiscript::fun(&ScriptObject::preSolveComponentsFun), "preSolveComponentsFun");
		chai.add(chaiscript::fun(&ScriptObject::postSolveComponentsFun), "postSolveComponentsFun");

	}

}

//#include <rttr/registration>
//RTTR_REGISTRATION
//{
//	using namespace rttr;
//	registration::class_<fse::ScriptObject>("fse::ScriptObject");
//}
