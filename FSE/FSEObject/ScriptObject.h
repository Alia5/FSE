#pragma once
#include "FSEObject.h"

namespace fse
{
	class ScriptObject : public FSEObject
	{
	public:
		explicit ScriptObject();
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

		void BeginContact(std::weak_ptr<FSEObject> otherObject, b2Contact* contact) override;
		void EndContact(std::weak_ptr<FSEObject> otherObject, b2Contact* contact) override;
		void PreSolve(std::weak_ptr<FSEObject> otherObject, b2Contact* contact, const b2Manifold* oldManifold) override;
		void PostSolve(std::weak_ptr<FSEObject> otherObject, b2Contact* contact, const b2ContactImpulse* impulse) override;

		bool destroy() override;
	protected:
		void BeginContactComponents(std::weak_ptr<FSEObject> otherObject, b2Contact* contact) override;
		void EndContactComponents(std::weak_ptr<FSEObject> otherObject, b2Contact* contact) override;
		void PreSolveComponents(std::weak_ptr<FSEObject> otherObject, b2Contact* contact, const b2Manifold* oldManifold) override;
		void PostSolveComponents(std::weak_ptr<FSEObject> otherObject, b2Contact* contact, const b2ContactImpulse* impulse) override;

	private:
		v8::Persistent < v8::Object, v8::CopyablePersistentTraits<v8::Object>> child_;

		std::string scriptClassName;

		friend class Scene;

		FSE_V8_ENABLE(ScriptObject);
		FSE_V8_REGISTRATION_FRIEND

		RTTR_ENABLE(fse::FSEObject)
		RTTR_REGISTRATION_FRIEND

	};


}