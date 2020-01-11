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
		if (!child_.IsEmpty())
		{
			auto meh = child_.Get(iso)->GetPropertyNames(ctx).ToLocalChecked();
			auto blaaa = v8pp::from_v8<std::vector<std::string>>(iso, meh);

			for (auto fu : blaaa)
				std::cout << fu << "\n";

			auto getId = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "getID"));
			
		}
	}

	
	void ScriptObject::onDespawn()
	{
	}

	void ScriptObject::update(float deltaTime)
	{
		auto iso = v8::Isolate::GetCurrent();
		auto hscope = v8::HandleScope(v8::Isolate::GetCurrent());
		auto ctx = iso->GetCurrentContext();
		auto update = child_.Get(iso)->Get(ctx, v8pp::to_v8(iso, "update"));
		if (!update.IsEmpty())
		{
			v8::Local<v8::Value> argv[] = { v8pp::to_v8(iso, deltaTime) };
			update.ToLocalChecked().As<v8::Function>()->Call(ctx, child_.Get(iso), 1, argv);
		} else
		{
			FSEObject::update(deltaTime);
		}
	}

	void ScriptObject::draw(sf::RenderTarget& target)
	{
	}

	void ScriptObject::drawNormals(sf::RenderTarget& target)
	{
	}

	void ScriptObject::drawSpecular(sf::RenderTarget& target)
	{
	}

	void ScriptObject::setPosition(const sf::Vector2f position)
	{
	}

	sf::Vector2f ScriptObject::getPosition()
	{
		return FSEObject::getPosition();
	}

	sf::FloatRect ScriptObject::GetAABBs() const
	{
		return FSEObject::GetAABBs();
	}

	void ScriptObject::BeginContact(FSEObject* otherObject, b2Contact* contact)
	{
	}

	void ScriptObject::EndContact(FSEObject* otherObject, b2Contact* contact)
	{
	}

	void ScriptObject::PreSolve(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold)
	{
	}

	void ScriptObject::PostSolve(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
	{
	}

	bool ScriptObject::destroy()
	{
		return FSEObject::destroy();
	}

	void ScriptObject::BeginContactComponents(FSEObject* otherObject, b2Contact* contact)
	{
	}

	void ScriptObject::EndContactComponents(FSEObject* otherObject, b2Contact* contact)
	{
	}

	void ScriptObject::PreSolveComponents(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold)
	{
	}

	void ScriptObject::PostSolveComponents(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
	{
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
						This->FSEObject::BeginContact(otherObject.get(), contact);
					});
				supermod.function("EndContact", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						auto otherObject = v8pp::from_v8<std::shared_ptr<FSEObject>>(iso, args[0]);
						auto contact = v8pp::from_v8<b2Contact*>(iso, args[1]);
						This->FSEObject::EndContact(otherObject.get(), contact);
					});
				supermod.function("PreSolve", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						auto otherObject = v8pp::from_v8<std::shared_ptr<FSEObject>>(iso, args[0]);
						auto contact = v8pp::from_v8<b2Contact*>(iso, args[1]);
						auto oldManifold = v8pp::from_v8<b2Manifold*>(iso, args[2]);
						This->FSEObject::PreSolve(otherObject.get(), contact, oldManifold);
					});
				supermod.function("PostSolve", [This](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						auto iso = v8::Isolate::GetCurrent();
						auto ctx = iso->GetCurrentContext();
						auto otherObject = v8pp::from_v8<std::shared_ptr<FSEObject>>(iso, args[0]);
						auto contact = v8pp::from_v8<b2Contact*>(iso, args[1]);
						auto impulse = v8pp::from_v8<b2ContactImpulse*>(iso, args[2]);
						This->FSEObject::PostSolve(otherObject.get(), contact, impulse);
					});
				auto object = args[0].As<v8::Object>();
				object->Set(ctx, v8pp::to_v8(iso, "super"), supermod.new_instance());
				v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>> persistentJS(iso, object);
				This->child_ = persistentJS;
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
	;
}