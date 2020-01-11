#include "ScriptObject.h"
#include <rttr/registration>

namespace fse
{
	ScriptObject::ScriptObject()
	{
	}

	ScriptObject::ScriptObject(v8::Persistent < v8::Object, v8::CopyablePersistentTraits<v8::Object>> object) : ScriptObject({ 0,0 })
	{
		child_ = object;
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
			
			//auto val = child_->Get(ctx, v8pp::to_v8(iso, "testString"));
			//if (!val.IsEmpty())
			//{
			//	std::string meh = v8pp::from_v8<std::string>(iso, val.ToLocalChecked());
			//}
			////if (child_->HasOwnProperty(v8::Isolate::GetCurrent()->GetCurrentContext(),
			////	v8::Local<v8::Name>(v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "testString").ToLocalChecked())).FromJust())
			////{
			////	std::cout << "test has property!\n";
			////}
		}
	}

	
	void ScriptObject::onDespawn()
	{
	}

	void ScriptObject::update(float deltaTime)
	{
		
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
				auto iso = v8::Isolate::GetCurrent();
				auto ctx = iso->GetCurrentContext();
				// TODO: overloads
				auto test = args.This();
				//auto test = args.Data();
				v8::Local<v8::Object> v8Object =
					test->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked();
				if (test->IsObject())
				{
					std::cout << "test is object\n";
					auto meh = v8Object->GetPropertyNames(ctx).ToLocalChecked();
					auto blaaa = v8pp::from_v8<std::vector<std::string>>(iso, meh);

					for (auto fu : blaaa)
						std::cout << fu << "\n";
				}
				v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>> persistentThis(iso, v8Object);

				return std::make_shared<ScriptObject>(persistentThis);
			});
		ScriptObject_class.function("setScriptChild", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
				auto iso = v8::Isolate::GetCurrent();
				auto ctx = iso->GetCurrentContext();
				// TODO: overloads
				auto test = args[0];
				v8::Local<v8::Object> v8Object =
					test->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked();
				if (test->IsObject())
				{
					std::cout << "test is object\n";
					auto meh = v8Object->GetPropertyNames(ctx).ToLocalChecked();
					auto blaaa = v8pp::from_v8<std::vector<std::string>>(iso, meh);

					for (auto fu : blaaa)
						std::cout << fu << "\n";
				}
				v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>> persistentThis(iso, v8Object);
				v8pp::from_v8<std::shared_ptr<fse::ScriptObject>>(iso, args.This())->child_ = persistentThis;
		});
		module.class_("ScriptObject", ScriptObject_class);
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