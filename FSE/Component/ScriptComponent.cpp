#include "ScriptComponent.h"
#include <rttr/registration>
#include "../FSEObject/FSEObject.h"

namespace fse
{
	ScriptComponent::ScriptComponent()
	{
	}

	void ScriptComponent::update(float deltaTime)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "update"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			v8::Local<v8::Value> argv[] = { v8pp::to_v8(iso, deltaTime) };
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 1, argv);
		}
	}
	
	void ScriptComponent::onAttach()
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "onAttach"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 0, nullptr);
		}
	}


	void ScriptComponent::onDetach()
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "onDetach"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 0, nullptr);
		}
	}

	void ScriptComponent::BeginContact(FSEObject* otherObject, b2Contact* contact)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "BeginContact"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			v8::Local<v8::Value> argv[] = { v8pp::to_v8(iso, otherObject), v8pp::to_v8(iso, contact) };
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 2, argv);
		}
	}

	void ScriptComponent::EndContact(FSEObject* otherObject, b2Contact* contact)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "EndContact"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			v8::Local<v8::Value> argv[] = { v8pp::to_v8(iso, otherObject), v8pp::to_v8(iso, contact) };
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 2, argv);
		}
	}

	void ScriptComponent::PreSolve(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "PreSolve"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			v8::Local<v8::Value> argv[] = { v8pp::to_v8(iso, otherObject), v8pp::to_v8(iso, contact), v8pp::to_v8(iso, oldManifold) };
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 3, argv);
		}
	}

	void ScriptComponent::PostSolve(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "PostSolve"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			v8::Local<v8::Value> argv[] = { v8pp::to_v8(iso, otherObject), v8pp::to_v8(iso, contact), v8pp::to_v8(iso, impulse) };
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 3, argv);
		}
	}

	FSE_V8_REGISTER(ScriptComponent)
	{
		v8::HandleScope handle_scope(isolate);
		v8pp::class_<ScriptComponent, v8pp::shared_ptr_traits>ScriptComponent_class(isolate);
		ScriptComponent_class.inherit<Component>();
		ScriptComponent_class.auto_wrap_objects(true);
		fse::addV8DownCastHelper<fse::Component, fse::ScriptComponent>();
		ScriptComponent_class.ctor<>(
			[](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				return std::make_shared<ScriptComponent>();
			});
		ScriptComponent_class.function("extend", [](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				auto iso = v8::Isolate::GetCurrent();
				auto ctx = iso->GetCurrentContext();
				auto This = v8pp::from_v8<std::shared_ptr<fse::ScriptComponent>>(iso, args.This());
				auto object = args[0].As<v8::Object>();
				const v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>> persistentJS(iso, object);
				This->child_ = persistentJS;
				This->scriptClassName = v8pp::from_v8<std::string>(iso,
					persistentJS.Get(iso)->Get(ctx, v8pp::to_v8(iso, "classname")).ToLocalChecked());
			});
		ScriptComponent_class.function("jsComponent", [](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				auto iso = args.GetIsolate();
				auto This = v8pp::from_v8<std::shared_ptr<ScriptComponent>>(iso, args.This());
				return This->child_.Get(iso);
			});
		module.class_("ScriptComponentNative", ScriptComponent_class);
	}
	
}

RTTR_REGISTRATION
{
	using namespace rttr;

	registration::class_<fse::ScriptComponent>("fse::ScriptComponent")
	.property_readonly("className", &fse::ScriptComponent::scriptClassName);
	;
}