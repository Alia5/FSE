//#pragma once
//#include "FSEObject.h"
//
//namespace fse
//{
//	class ScriptObject : public FSEObject
//	{
//	public:
//		explicit ScriptObject(const std::string& scriptName);
//		explicit ScriptObject(const sf::Vector2f& spawnPos, const std::string& scriptName);
//		~ScriptObject() = default;
//
//		void spawned() override;
//		void onDespawn() override;
//
//		void update(float deltaTime) override;
//
//		void draw(sf::RenderTarget& target) override;
//		void drawNormals(sf::RenderTarget& target) override;
//		void drawSpecular(sf::RenderTarget& target) override;
//
//		void setPosition(const sf::Vector2f position) override;
//		sf::Vector2f getPosition() override;
//		sf::FloatRect GetAABBs() const override;
//
//		void BeginContact(FSEObject* otherObject, b2Contact* contact) override;
//		void EndContact(FSEObject* otherObject, b2Contact* contact) override;
//		void PreSolve(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold) override;
//		void PostSolve(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse) override;
//
//		bool destroy() override;
//	protected:
//		void BeginContactComponents(FSEObject* otherObject, b2Contact* contact) override;
//		void EndContactComponents(FSEObject* otherObject, b2Contact* contact) override;
//		void PreSolveComponents(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold) override;
//		void PostSolveComponents(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse) override;
//
//	private:
//
//		std::string script_name_;
//		bool initialized_ = false;
//		chaiscript::Boxed_Value script_child_;
//
//
//		mutable std::function<void(const chaiscript::Boxed_Value&)> spawnedFun = nullptr;
//		mutable std::function<void(const chaiscript::Boxed_Value&)> onDespawnFun = nullptr;
//
//
//		mutable std::function<void(const chaiscript::Boxed_Value&, float)> updateFun = nullptr;
//		mutable std::function<void(const chaiscript::Boxed_Value&, sf::RenderTarget&)> drawFun = nullptr;
//		mutable std::function<void(const chaiscript::Boxed_Value&, sf::RenderTarget&)> drawNormalsFun = nullptr;
//		mutable std::function<void(const chaiscript::Boxed_Value&, sf::RenderTarget&)> drawSpecularFun = nullptr;
//
//
//		mutable std::function<void(const chaiscript::Boxed_Value&, const sf::Vector2f)> setPositionFun = nullptr;
//		mutable std::function<sf::Vector2f(const chaiscript::Boxed_Value&)> getPositionFun = nullptr;
//		mutable std::function<sf::FloatRect(const chaiscript::Boxed_Value&)> getAABBsFun = nullptr;
//
//
//		mutable std::function<void(const chaiscript::Boxed_Value&, FSEObject*, b2Contact*)> beginContactFun = nullptr;
//		mutable std::function<void(const chaiscript::Boxed_Value&, FSEObject*, b2Contact*)> endContactFun = nullptr;
//
//		mutable std::function<void(const chaiscript::Boxed_Value&, FSEObject*, b2Contact*, const b2Manifold*)> preSolveFun = nullptr;
//		mutable std::function<void(const chaiscript::Boxed_Value&, FSEObject*, b2Contact*, const b2ContactImpulse*)> postSolveFun = nullptr;
//
//		mutable std::function<bool(const chaiscript::Boxed_Value&)> destroyFun = nullptr;
//
//
//		mutable std::function<void(const chaiscript::Boxed_Value&, FSEObject*, b2Contact*)> beginContactComponentsFun = nullptr;
//		mutable std::function<void(const chaiscript::Boxed_Value&, FSEObject*, b2Contact*)> endContactComponentsFun = nullptr;
//
//		mutable std::function<void(const chaiscript::Boxed_Value&, FSEObject*, b2Contact*, const b2Manifold*)> preSolveComponentsFun = nullptr;
//		mutable std::function<void(const chaiscript::Boxed_Value&, FSEObject*, b2Contact*, const b2ContactImpulse*)> postSolveComponentsFun = nullptr;
//
//		FSE_CHAI_ENABLE(ScriptObject);
//		FSE_CHAI_REGISTRATION_FRIEND
//
//		RTTR_ENABLE(fse::FSEObject)
//		RTTR_REGISTRATION_FRIEND
//
//	};
//
//
//}