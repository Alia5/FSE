#include "ScriptObject.h"
#include <rttr/registration>

namespace fse
{
	ScriptObject::ScriptObject()
	{
	}

	ScriptObject::ScriptObject(const sf::Vector2f& spawnPos) : FSEObject(spawnPos)
	{
	}

	void ScriptObject::spawned()
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "spawned"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 0, nullptr);
		}
	}

	
	void ScriptObject::onDespawn()
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "onDespawn"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 0, nullptr);
		}
	}

	void ScriptObject::update(float deltaTime)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "update"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			v8::Local<v8::Value> argv[] = { v8pp::to_v8(iso, deltaTime) };
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 1, argv);
		} else
		{
			FSEObject::update(deltaTime);
		}
	}

	void ScriptObject::draw(sf::RenderTarget& target)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "draw"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			v8::Local<v8::Value> argv[] = { Renderer::_v8_render_target };
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 1, argv);
		}
	}

	void ScriptObject::drawNormals(sf::RenderTarget& target)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "drawNormals"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			v8::Local<v8::Value> argv[] = { v8pp::to_v8(iso, &target) };
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 1, argv);
		}
	}

	void ScriptObject::drawSpecular(sf::RenderTarget& target)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "drawSpecular"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			v8::Local<v8::Value> argv[] = { v8pp::to_v8(iso, &target) };
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 1, argv);
		}
	}

	void ScriptObject::setPosition(const sf::Vector2f position)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "setPosition"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			v8::Local<v8::Value> argv[] = { v8pp::to_v8(iso, position) };
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 1, argv);
		}
		else
		{
			FSEObject::setPosition(position);
		}
	}

	sf::Vector2f ScriptObject::getPosition()
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "getPosition"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			return v8pp::from_v8<sf::Vector2f>(iso,
				func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 0, nullptr)
				.ToLocalChecked());
		}
		else
		{
			return FSEObject::getPosition();
		}
	}

	sf::FloatRect ScriptObject::GetAABBs() const
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "GetAABBs"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			return v8pp::from_v8<sf::FloatRect>(iso,
				func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 0, nullptr)
				.ToLocalChecked());
		}
		else
		{
			return FSEObject::GetAABBs();
		}
	}

	void ScriptObject::BeginContact(std::weak_ptr<FSEObject> otherObject, b2Contact* contact)
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
		else
		{
			return FSEObject::BeginContact(otherObject, contact);
		}
	}

	void ScriptObject::EndContact(std::weak_ptr<FSEObject> otherObject, b2Contact* contact)
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
		else
		{
			return FSEObject::EndContact(otherObject, contact);
		}
	}

	void ScriptObject::PreSolve(std::weak_ptr<FSEObject> otherObject, b2Contact* contact, const b2Manifold* oldManifold)
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
		else
		{
			return FSEObject::PreSolve(otherObject, contact, oldManifold);
		}
	}

	void ScriptObject::PostSolve(std::weak_ptr<FSEObject> otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
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
		else
		{
			return FSEObject::PostSolve(otherObject, contact, impulse);
		}
	}

	bool ScriptObject::destroy()
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "destroy"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			return v8pp::from_v8<bool>(iso,
				func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 0, nullptr)
				.ToLocalChecked());
		}
		else
		{
			return FSEObject::destroy();
		}
	}

	void ScriptObject::BeginContactComponents(std::weak_ptr<FSEObject> otherObject, b2Contact* contact)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "BeginContactComponents"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			v8::Local<v8::Value> argv[] = { v8pp::to_v8(iso, otherObject), v8pp::to_v8(iso, contact) };
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 2, argv);
		}
		else
		{
			return FSEObject::BeginContactComponents(otherObject, contact);
		}
	}

	void ScriptObject::EndContactComponents(std::weak_ptr<FSEObject> otherObject, b2Contact* contact)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "EndContactComponents"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			v8::Local<v8::Value> argv[] = { v8pp::to_v8(iso, otherObject), v8pp::to_v8(iso, contact) };
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 2, argv);
		}
		else
		{
			return FSEObject::EndContactComponents(otherObject, contact);
		}
	}

	void ScriptObject::PreSolveComponents(std::weak_ptr<FSEObject> otherObject, b2Contact* contact, const b2Manifold* oldManifold)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "PreSolveComponents"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			v8::Local<v8::Value> argv[] = { v8pp::to_v8(iso, otherObject), v8pp::to_v8(iso, contact), v8pp::to_v8(iso, oldManifold) };
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 3, argv);
		}
		else
		{
			return FSEObject::PreSolveComponents(otherObject, contact, oldManifold);
		}
	}

	void ScriptObject::PostSolveComponents(std::weak_ptr<FSEObject> otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto func = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "PostSolveComponents"));
		if (!func.IsEmpty() && func.ToLocalChecked()->IsFunction())
		{
			v8::Local<v8::Value> argv[] = { v8pp::to_v8(iso, otherObject), v8pp::to_v8(iso, contact), v8pp::to_v8(iso, impulse) };
			func.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 3, argv);
		}
		else
		{
			return FSEObject::PostSolveComponents(otherObject, contact, impulse);
		}
	}

	FSE_V8_REGISTER(ScriptObject)
	{
		v8::HandleScope handle_scope(isolate);
		v8pp::class_<ScriptObject, v8pp::shared_ptr_traits>ScriptObject_class(isolate);
		ScriptObject_class.inherit<FSEObject>();
		fse::addV8DownCastHelper<fse::FSEObject, fse::ScriptObject>();
		ScriptObject_class.auto_wrap_objects(true);
		ScriptObject_class.ctor<>(
			[](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				return std::make_shared<ScriptObject>();
			});
		ScriptObject_class.function("update", &ScriptObject::update);


		ScriptObject_class.function("extend", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
				auto iso = v8::Isolate::GetCurrent();
				auto ctx = iso->GetCurrentContext();

				auto This = v8pp::from_v8<std::shared_ptr<fse::ScriptObject>>(iso, args.This());
			
				v8pp::module supermod(iso);
				supermod.function("update", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						auto deltaTime = v8pp::from_v8<float>(iso, args[0]);
						This->FSEObject::update(deltaTime);
					});
				supermod.function("setPosition", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						auto position = v8pp::from_v8<sf::Vector2f>(iso, args[0]);
						This->FSEObject::setPosition(position);
					});
				supermod.function("getPosition", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						return This->FSEObject::getPosition();
					});
				supermod.function("GetAABBs", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						return This->FSEObject::GetAABBs();
					});
				supermod.function("BeginContact", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						auto otherObject = v8pp::from_v8<std::shared_ptr<FSEObject>>(iso, args[0]);
						auto contact = v8pp::from_v8<b2Contact*>(iso, args[1]);
						This->FSEObject::BeginContact(otherObject, contact);
					});
				supermod.function("EndContact", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						auto otherObject = v8pp::from_v8<std::shared_ptr<FSEObject>>(iso, args[0]);
						auto contact = v8pp::from_v8<b2Contact*>(iso, args[1]);
						This->FSEObject::EndContact(otherObject, contact);
					});
				supermod.function("PreSolve", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						auto otherObject = v8pp::from_v8<std::shared_ptr<FSEObject>>(iso, args[0]);
						auto contact = v8pp::from_v8<b2Contact*>(iso, args[1]);
						auto oldManifold = v8pp::from_v8<b2Manifold*>(iso, args[2]);
						This->FSEObject::PreSolve(otherObject, contact, oldManifold);
					});
				supermod.function("PostSolve", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						auto otherObject = v8pp::from_v8<std::shared_ptr<FSEObject>>(iso, args[0]);
						auto contact = v8pp::from_v8<b2Contact*>(iso, args[1]);
						auto impulse = v8pp::from_v8<b2ContactImpulse*>(iso, args[2]);
						This->FSEObject::PostSolve(otherObject, contact, impulse);
					});
				supermod.function("destroy", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						return This->FSEObject::destroy();
					});
				supermod.function("BeginContactComponents", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						auto otherObject = v8pp::from_v8<std::shared_ptr<FSEObject>>(iso, args[0]);
						auto contact = v8pp::from_v8<b2Contact*>(iso, args[1]);
						This->FSEObject::BeginContactComponents(otherObject, contact);
					});
				supermod.function("EndContactComponents", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						auto otherObject = v8pp::from_v8<std::shared_ptr<FSEObject>>(iso, args[0]);
						auto contact = v8pp::from_v8<b2Contact*>(iso, args[1]);
						This->FSEObject::EndContactComponents(otherObject, contact);
					});
				supermod.function("PreSolveComponents", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						const auto otherObject = v8pp::from_v8<std::shared_ptr<FSEObject>>(iso, args[0]);
						const auto contact = v8pp::from_v8<b2Contact*>(iso, args[1]);
						const auto oldManifold = v8pp::from_v8<b2Manifold*>(iso, args[2]);
						This->FSEObject::PreSolveComponents(otherObject, contact, oldManifold);
					});
				supermod.function("PostSolveComponents", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						const auto otherObject = v8pp::from_v8<std::shared_ptr<FSEObject>>(iso, args[0]);
						const auto contact = v8pp::from_v8<b2Contact*>(iso, args[1]);
						const auto impulse = v8pp::from_v8<b2ContactImpulse*>(iso, args[2]);
						This->FSEObject::PostSolveComponents(otherObject, contact, impulse);
					});
				auto object = args[0].As<v8::Object>();
				object->Set(ctx, v8pp::to_v8(iso, "super"), supermod.new_instance());
				const v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>> persistentJS(iso, object);
				This->child_ = persistentJS;
				This->scriptClassName = v8pp::from_v8<std::string>(iso,
					persistentJS.Get(iso)->Get(ctx, v8pp::to_v8(iso, "classname")).ToLocalChecked());
		});
		ScriptObject_class.function("jsObject", [](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				auto iso = args.GetIsolate();
				auto This = v8pp::from_v8<std::shared_ptr<ScriptObject>>(iso, args.This());
				return This->child_.Get(iso);
			});
		module.class_("ScriptObjectNative", ScriptObject_class);
	}
	
}



RTTR_REGISTRATION
{
	using namespace rttr;

	registration::class_<fse::ScriptObject>("fse::ScriptObject")
	//.constructor<>()
	//(
	//policy::ctor::as_std_shared_ptr
	//)
	//.constructor<const sf::Vector2f&>()
	//(
	//	policy::ctor::as_std_shared_ptr,
	//	parameter_names("spawn position")
	//)
	.property_readonly("className", &fse::ScriptObject::scriptClassName);
	;
}