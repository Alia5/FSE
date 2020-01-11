#pragma once
#include "FSEObject.h"

namespace fse
{
	class ScriptObject : public FSEObject
	{
	public:
		explicit ScriptObject();
		explicit ScriptObject(v8::Persistent < v8::Object, v8::CopyablePersistentTraits<v8::Object>> object);
		explicit ScriptObject(const sf::Vector2f& spawnPos);
		~ScriptObject() = default;

		void spawned() override;
		void onDespawn() override;

		void update(float deltaTime) override;

		void draw(sf::RenderTarget& target) override;
		void drawNormals(sf::RenderTarget& target) override;
		void drawSpecular(sf::RenderTarget& target) override;

		void setPosition(const sf::Vector2f position) override;
		sf::Vector2f getPosition() override;
		sf::FloatRect GetAABBs() const override;

		void BeginContact(FSEObject* otherObject, b2Contact* contact) override;
		void EndContact(FSEObject* otherObject, b2Contact* contact) override;
		void PreSolve(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold) override;
		void PostSolve(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse) override;

		bool destroy() override;
	protected:
		void BeginContactComponents(FSEObject* otherObject, b2Contact* contact) override;
		void EndContactComponents(FSEObject* otherObject, b2Contact* contact) override;
		void PreSolveComponents(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold) override;
		void PostSolveComponents(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse) override;

	private:

		std::string script_name_;
		bool initialized_ = false;

		v8::Persistent < v8::Object, v8::CopyablePersistentTraits<v8::Object>> child_;

		FSE_V8_ENABLE(ScriptObject);
		FSE_V8_REGISTRATION_FRIEND

		RTTR_ENABLE(fse::FSEObject)
		RTTR_REGISTRATION_FRIEND

	};


}