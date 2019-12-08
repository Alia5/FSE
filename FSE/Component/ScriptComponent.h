//#pragma once
//#include "Component.h"
//
//namespace fse
//{
//	class ScriptComponent : public Component
//	{
//	public:
//		explicit ScriptComponent(const std::string& script_name);
//		virtual ~ScriptComponent() = default;
//
//
//		void update(float deltaTime) override;
//
//		void onAttach() override;
//		void onDetach() override;
//		void BeginContact(FSEObject* otherObject, b2Contact* contact) override;
//		void EndContact(FSEObject* otherObject, b2Contact* contact) override;
//		void PreSolve(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold) override;
//		void PostSolve(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse) override;
//
//
//	private:
//		std::string script_name_;
//		bool initialized_ = false;
//		chaiscript::Boxed_Value script_child_;
//
//		mutable std::function<void(const chaiscript::Boxed_Value&, float)> updateFun = nullptr;
//		mutable std::function<void(const chaiscript::Boxed_Value&)> onAttachFun = nullptr;
//		mutable std::function<void(const chaiscript::Boxed_Value&)> onDetachFun = nullptr;
//
//		mutable std::function<void(const chaiscript::Boxed_Value&, FSEObject*, b2Contact*)> beginContactFun = nullptr;
//		mutable std::function<void(const chaiscript::Boxed_Value&, FSEObject*, b2Contact*)> endContactFun = nullptr;
//
//		mutable std::function<void(const chaiscript::Boxed_Value&, FSEObject*, b2Contact*, const b2Manifold*)> preSolveFun = nullptr;
//		mutable std::function<void(const chaiscript::Boxed_Value&, FSEObject*, b2Contact*, const b2ContactImpulse*)> postSolveFun = nullptr;
//
//		FSE_CHAI_ENABLE(ScriptComponent);
//		FSE_CHAI_REGISTRATION_FRIEND
//
//		RTTR_ENABLE(fse::Component)
//		RTTR_REGISTRATION_FRIEND
//	};
//
//}
