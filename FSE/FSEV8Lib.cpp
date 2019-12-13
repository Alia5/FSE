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
#include <v8pp/module.hpp>
#include <functional>

//#include <chaiscript/extras/math.hpp>
//#include <chaiscript/extras/string_methods.hpp>
//#include <chaiscript/extras/box2d.hpp>
//#include <chaiscript/extras/sfml.hpp>


#include <v8pp/module.hpp>
#include <v8pp/class.hpp>

namespace fse
{
	namespace priv
	{
		void FSEV8Lib::Init(v8::Isolate* isolate)
		{

				v8::HandleScope handle_scope(isolate);
				v8pp::module module(isolate);
				v8pp::class_<sf::Vector2f> vector2fClass(isolate);
				vector2fClass.var("x", &sf::Vector2f::x);
				vector2fClass.var("y", &sf::Vector2f::y);


				v8pp::class_<FSEObject> FSEObject_class(isolate);
				FSEObject_class.function("getID", &FSEObject::getID);
				//Scene_class.function("setPaused", &Scene::setPaused);
				//Scene_class.function("getPhysDrawDebug", &Scene::getPhysDrawDebug);
				//Scene_class.function("setPhysDrawDebug", &Scene::setPhysDrawDebug);
				//Scene_class.function("getLightWorld", &Scene::getLightWorld);
				//Scene_class.function("getPhysWorld", &Scene::getPhysWorld);
				//Scene_class.function("getPixelsPerMeter", &Scene::getPixelsPerMeter);
				//Scene_class.function("getMetersPerPixel", &Scene::getMetersPerPixel);
				//Scene_class.function("getRenderTarget", &Scene::getRenderTarget);
				//Scene_class.function("spawnObject", &Scene::spawnObject);

				module.class_("FSEObject", FSEObject_class);

				//v8pp::class_<FSEObject, v8pp::shared_ptr_traits> FSEObject_shared_class(isolate);
				//FSEObject_shared_class.function("getID", &FSEObject::getID);

				//module.class_("SharedFSEObject", FSEObject_shared_class);

				
				v8pp::class_<FSELightWorld>FSELightWorld_class(isolate);
				FSELightWorld_class.inherit<FSEObject>();
				FSELightWorld_class.var("lighting", &FSELightWorld::lighting_);
				//Scene_class.function("setPaused", &Scene::setPaused);
				//Scene_class.function("getPhysDrawDebug", &Scene::getPhysDrawDebug);
				//Scene_class.function("setPhysDrawDebug", &Scene::setPhysDrawDebug);
				//Scene_class.function("getLightWorld", &Scene::getLightWorld);
				//Scene_class.function("getPhysWorld", &Scene::getPhysWorld);
				//Scene_class.function("getPixelsPerMeter", &Scene::getPixelsPerMeter);
				//Scene_class.function("getMetersPerPixel", &Scene::getMetersPerPixel);
				//Scene_class.function("getRenderTarget", &Scene::getRenderTarget);
				//Scene_class.function("spawnObject", &Scene::spawnObject);

				module.class_("FSELightWorld_class", FSELightWorld_class);

				//v8pp::class_<FSEObject, v8pp::shared_ptr_traits> FSEObject_shared_class(isolate);
				//FSEObject_shared_class.function("getID", &FSEObject::getID);

				//module.class_("SharedFSEObject", FSEObject_shared_class);

				v8pp::class_<Scene> Scene_class(isolate);
				Scene_class.function("isPaused", &Scene::isPaused);
				Scene_class.function("setPaused", &Scene::setPaused);
				Scene_class.function("getPhysDrawDebug", &Scene::getPhysDrawDebug);
				Scene_class.function("setPhysDrawDebug", &Scene::setPhysDrawDebug);
				Scene_class.function("getLightWorld", &Scene::getLightWorld);
				Scene_class.function("getPhysWorld", &Scene::getPhysWorld);
				Scene_class.function("getPixelsPerMeter", &Scene::getPixelsPerMeter);
				Scene_class.function("getMetersPerPixel", &Scene::getMetersPerPixel);
				Scene_class.function("getRenderTarget", &Scene::getRenderTarget);
				Scene_class.function("test", [isolate](Scene* scene)
					{
						typedef v8pp::class_<fse::FSEObject> my_class_wrapper;
						v8::Local<v8::Value> val = my_class_wrapper::import_external(isolate, (scene->fse_objects_[0].get()));
						return val;
					});
				//Scene_class.function("spawnObject", &Scene::spawnObject);

				module.class_("Scene", Scene_class);

				// set bindings in global object as `mylib`
				isolate->GetCurrentContext()->Global()->Set(isolate->GetCurrentContext(),
					v8::String::NewFromUtf8(isolate, "fse").ToLocalChecked(), module.new_instance());
			

				//// set bindings in global object as `mylib`
				//isolate->GetCurrentContext()->Global()->Set(isolate->GetCurrentContext(),
				//	v8::String::NewFromUtf8(isolate, "fseObject").ToLocalChecked(), module.new_instance());
			
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

				v8_init::execute(isolate);
		}

	}
}
