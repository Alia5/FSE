#include "FSEV8Lib.h"
#include "FSEObject/KillVolume.h"
#include "Lights/FSELightWorld.h"
#include "FSEObject/FPSCounter.h"
#include "FSEObject/Timer.h"
#include "Lights/Light.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "Random.h"
#include "Scene.h"
#include <functional>


namespace fse
{
	namespace priv
	{
		void FSEV8Lib::Init(v8::Isolate* isolate)
		{

				v8::HandleScope handle_scope(isolate);
				v8pp::module module(isolate);
			
				////chai.add(chaiscript::extras::math::bootstrap());
				////chai.add(chaiscript::extras::string_methods::bootstrap());

				////chai.add(chaiscript::extras::box2d::bootstrap());

				////chai.add(chaiscript::fun([](const b2Body* body)
				//{
				//	return static_cast<FSEObject*>(body->GetUserData());
				//}), "GetUserData");
				////chai.add(chaiscript::fun([](b2Body* body, FSEObject* data)
				//{
				//	body->SetUserData(data);
				//}), "SetUserData");

				////chai.add(chaiscript::fun([](const b2Fixture* fixture)
				//{
				//	return static_cast<FSEObject*>(fixture->GetUserData());
				//}), "GetUserData");
				////chai.add(chaiscript::fun([](b2Fixture* fixture, FSEObject* data)
				//{
				//	fixture->SetUserData(data);
				//}), "SetUserData");


				////chai.add(chaiscript::fun([](const b2ParticleDef* pdef)
				//{
				//	return static_cast<FSEObject*>(pdef->userData);
				//}), "GetUserData");
				////chai.add(chaiscript::fun([](b2ParticleDef* pdef, FSEObject* data)
				//{
				//	pdef->userData = data;
				//}), "SetUserData");


				////chai.add(chaiscript::fun([](const b2ParticleGroupDef* pdef)
				//{
				//	return static_cast<FSEObject*>(pdef->userData);
				//}), "GetUserData");
				////chai.add(chaiscript::fun([](b2ParticleGroupDef* pdef, FSEObject* data)
				//{
				//	pdef->userData = data;
				//}), "SetUserData");

				////chai.add(chaiscript::fun([](const b2ParticleGroup* group)
				//{
				//	return static_cast<FSEObject*>(group->GetUserData());
				//}), "GetUserData");
				////chai.add(chaiscript::fun([](b2ParticleGroup* group, FSEObject* data)
				//{
				//	group->SetUserData(data);
				//}), "SetUserData");

				////chai.add(chaiscript::fun([](const b2ParticleSystem* system)
				//{
				//	return (FSEObject**)(system->GetUserDataBuffer());
				//}), "GetUserData");
				////chai.add(chaiscript::fun([](const b2ParticleSystem* system)
				//{
				//	return reinterpret_cast<FSEObject* const*>(system->GetUserDataBuffer());
				//}), "GetUserData");
				////chai.add(chaiscript::fun([](b2ParticleSystem* system, FSEObject** data, const int capacity)
				//{
				//	system->SetUserDataBuffer(reinterpret_cast<void**>(data), capacity);
				//}), "SetUserData");


				////chai.add(fse::Random::bootstrap());

				////chai.add(chaiscript::extras::sfml::bootstrap());

				////chai.add(chaiscript::type_conversion<sf::Vector2f, b2Vec2>([](const sf::Vector2f& vec) { return b2Vec2(vec.x, vec.y); }));
				////chai.add(chaiscript::type_conversion<b2Vec2, sf::Vector2f>([](const b2Vec2& vec) { return sf::Vector2f(vec.x, vec.y); }));

				////chai.add(chaiscript::fun([](sf::Vector2f& lhs, const b2Vec2& rhs) { return lhs = sf::Vector2f(rhs.x, rhs.y); }), "=");
				////chai.add(chaiscript::fun([](b2Vec2& lhs, const sf::Vector2f& rhs) { return lhs = b2Vec2(rhs.x, rhs.y); }), "=");

				////chai.add(chaiscript::fun([](const sf::Vector2f& lhs, const b2Vec2& rhs) { return lhs == sf::Vector2f(rhs.x, rhs.y); }), "==");
				////chai.add(chaiscript::fun([](const b2Vec2& lhs, const sf::Vector2f& rhs) { return lhs == b2Vec2(rhs.x, rhs.y); }), "==");


				////chai.add(chaiscript::vector_conversion<std::vector<std::shared_ptr<FSEObject>>>());
				////chai.add(chaiscript::bootstrap::standard_library::vector_type<std::vector<std::shared_ptr<FSEObject>>>("ObjectList"));


				////chai.add(chaiscript::vector_conversion<std::vector<std::weak_ptr<FSEObject>>>());
				////chai.add(chaiscript::bootstrap::standard_library::vector_type<std::vector<std::weak_ptr<FSEObject>>>("WeakObjectList"));

				////chai.add(chaiscript::vector_conversion<std::vector<Light*>>());
				////chai.add(chaiscript::bootstrap::standard_library::vector_type<std::vector<Light*>>("LightList"));

				v8_init::execute(isolate, module);
				isolate->GetCurrentContext()->Global()->Set(isolate->GetCurrentContext(),
					v8::String::NewFromUtf8(isolate, "fse").ToLocalChecked(), module.new_instance());

		}

	}
}
