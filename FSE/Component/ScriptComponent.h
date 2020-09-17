#pragma once
#include "Component.h"

namespace fse
{
	class ScriptComponent : public Component
	{
	public:
		ScriptComponent();
		~ScriptComponent() = default;
		void update(float deltaTime) override;
		void onAttach() override;
		void onDetach() override;
		void BeginContact(std::weak_ptr<FSEObject> otherObject, b2Contact* contact) override;
		void EndContact(std::weak_ptr<FSEObject> otherObject, b2Contact* contact) override;
		void PreSolve(std::weak_ptr<FSEObject> otherObject, b2Contact* contact, const b2Manifold* oldManifold) override;
		void PostSolve(std::weak_ptr<FSEObject> otherObject, b2Contact* contact, const b2ContactImpulse* impulse) override;
	private:
		v8::Persistent < v8::Object, v8::CopyablePersistentTraits<v8::Object>> child_;

		std::string scriptClassName;

		FSE_V8_ENABLE(ScriptComponent);
		FSE_V8_REGISTRATION_FRIEND

		RTTR_ENABLE(fse::Component)
		RTTR_REGISTRATION_FRIEND
	};

}
