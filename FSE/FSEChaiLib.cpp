#include "FSEChaiLib.h"
#include "FSEObject/KillVolume.h"
#include "Lights/FSELightWorld.h"
#include "FSEObject/FPSCounter.h"
#include "FSEObject/Timer.h"
#include "Lights/Light.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "Random.h"

namespace fse
{
	namespace priv
	{
		void FSEChaiLib::Init(chaiscript::ChaiScript& chai)
		{
			
				chai.add(fse::Random::bootstrap());
			
				chai.add(chaiscript::user_type<sf::Vector2f>(), "Vector2f");
				chai.add(chaiscript::fun(&sf::Vector2f::x), "x");
				chai.add(chaiscript::fun(&sf::Vector2f::y), "y");
				chai.add(chaiscript::constructor<sf::Vector2f()>(), "Vector2f");
				chai.add(chaiscript::constructor<sf::Vector2f(float, float)>(), "Vector2f");
				chai.add(chaiscript::constructor<sf::Vector2f(const sf::Vector2f&)>(), "Vector2f");
				chai.add(chaiscript::fun([](sf::Vector2f& lhs, const sf::Vector2f& rhs) { return lhs = rhs; }), "=");
				chai.add(chaiscript::fun([](sf::Vector2f& lhs, const sf::Vector2f& rhs) { return lhs + rhs; }), "+");
				chai.add(chaiscript::fun([](sf::Vector2f& lhs, const sf::Vector2f& rhs) { return lhs - rhs; }), "-");
				chai.add(chaiscript::fun([](sf::Vector2f& lhs, const sf::Vector2f& rhs) { return sf::Vector2f(lhs.x * rhs.x, lhs.y * rhs.y); }), "*");
				chai.add(chaiscript::fun([](sf::Vector2f& lhs, const sf::Vector2f& rhs) { return sf::Vector2f(lhs.x / rhs.x, lhs.y / rhs.y); }), "/");
				chai.add(chaiscript::fun([](sf::Vector2f& lhs, const float f) { return sf::Vector2f(lhs.x * f, lhs.y * f); }), "*");
				chai.add(chaiscript::fun([](sf::Vector2f& lhs, const float f) { return sf::Vector2f(lhs.x / f, lhs.y / f); }), "/");
				chai.add(chaiscript::fun([](const sf::Vector2f& lhs, const sf::Vector2f& rhs) { return lhs == rhs; }), "==");
				chai.add(chaiscript::fun([](const sf::Vector2f& vec) { return "{ x: " + std::to_string(vec.x) + ", y: " + std::to_string(vec.y) + " }"; }), "to_string");

				chai.add(chaiscript::user_type<sf::Vector2i>(), "Vector2i");
				chai.add(chaiscript::fun(&sf::Vector2i::x), "x");
				chai.add(chaiscript::fun(&sf::Vector2i::y), "y");
				chai.add(chaiscript::constructor<sf::Vector2i()>(), "Vector2i");
				chai.add(chaiscript::constructor<sf::Vector2i(float, float)>(), "Vector2i");
				chai.add(chaiscript::constructor<sf::Vector2i(const sf::Vector2i&)>(), "Vector2i");
				chai.add(chaiscript::fun([](sf::Vector2i& lhs, const sf::Vector2i& rhs) { return lhs = rhs; }), "=");
				chai.add(chaiscript::fun([](sf::Vector2i& lhs, const sf::Vector2i& rhs) { return lhs + rhs; }), "+");
				chai.add(chaiscript::fun([](sf::Vector2i& lhs, const sf::Vector2i& rhs) { return lhs - rhs; }), "-");
				chai.add(chaiscript::fun([](const sf::Vector2i& lhs, const sf::Vector2i& rhs) { return lhs == rhs; }), "==");
				chai.add(chaiscript::fun([](const sf::Vector2i& vec) { return "{ x: " + std::to_string(vec.x) + ", y: " + std::to_string(vec.y) + " }"; }), "to_string");


				chai.add(chaiscript::user_type<sf::IntRect>(), "IntRect");
				chai.add(chaiscript::fun(&sf::IntRect::top), "top");
				chai.add(chaiscript::fun(&sf::IntRect::left), "left");
				chai.add(chaiscript::fun(&sf::IntRect::width), "width");
				chai.add(chaiscript::fun(&sf::IntRect::height), "height");
				chai.add(chaiscript::constructor<sf::IntRect()>(), "IntRect");
				chai.add(chaiscript::constructor<sf::IntRect(int, int, int, int)>(), "IntRect");
				chai.add(chaiscript::constructor<sf::IntRect(const sf::Vector2i&, const sf::Vector2i&)>(), "IntRect");
				chai.add(chaiscript::constructor<sf::IntRect(const sf::IntRect&)>(), "IntRect");
				chai.add(chaiscript::fun([](const sf::IntRect* rect, int x, int y) {return rect->contains(x, y); }), "contains");
				chai.add(chaiscript::fun([](const sf::IntRect* rect, const sf::Vector2i& vec) {return rect->contains(vec); }), "contains");
				chai.add(chaiscript::fun([](const sf::IntRect* rect, const sf::IntRect& other) {return rect->intersects(other); }), "intersects");


				chai.add(chaiscript::user_type<sf::FloatRect>(), "FloatRect");
				chai.add(chaiscript::fun(&sf::FloatRect::top), "top");
				chai.add(chaiscript::fun(&sf::FloatRect::left), "left");
				chai.add(chaiscript::fun(&sf::FloatRect::width), "width");
				chai.add(chaiscript::fun(&sf::FloatRect::height), "height");
				chai.add(chaiscript::constructor<sf::FloatRect()>(), "FloatRect");
				chai.add(chaiscript::constructor<sf::FloatRect(float, float, float, float)>(), "FloatRect");
				chai.add(chaiscript::constructor<sf::FloatRect(const sf::Vector2f&, const sf::Vector2f&)>(), "FloatRect");
				chai.add(chaiscript::constructor<sf::FloatRect(const sf::FloatRect&)>(), "FloatRect");
				chai.add(chaiscript::fun([](const sf::FloatRect* rect, float x, float y) {return rect->contains(x, y); }), "contains");
				chai.add(chaiscript::fun([](const sf::FloatRect* rect, const sf::Vector2f& vec) {return rect->contains(vec); }), "contains");
				chai.add(chaiscript::fun([](const sf::FloatRect* rect, const sf::FloatRect& other) {return rect->intersects(other); }), "intersects");


				chai.add(chaiscript::user_type<sf::Color>(), "Color");
				chai.add(chaiscript::fun(&sf::Color::r), "r");
				chai.add(chaiscript::fun(&sf::Color::g), "g");
				chai.add(chaiscript::fun(&sf::Color::b), "b");
				chai.add(chaiscript::fun(&sf::Color::a), "a");
				chai.add(chaiscript::constructor<sf::Color()>(), "Color");
				chai.add(chaiscript::constructor<sf::Color(int, int, int, int)>(), "Color");
				chai.add(chaiscript::constructor<sf::Color(int, int, int)>(), "Color");
				chai.add(chaiscript::constructor<sf::Color(const sf::Color&)>(), "Color");
				chai.add(chaiscript::fun([](sf::Color& lhs, const sf::Color& rhs) { return lhs = rhs; }), "=");
				chai.add(chaiscript::fun([](const sf::Color& lhs, const sf::Color& rhs) { return lhs == rhs; }), "==");

				chai.add(chaiscript::user_type<Scene>(), "FSEScene");
				chai.add(chaiscript::fun(static_cast<bool(Scene::*)() const>(&Scene::isPaused)), "isPaused");
				chai.add(chaiscript::fun(static_cast<void(Scene::*)(bool)>(&Scene::setPaused)), "setPaused");
				chai.add(chaiscript::fun(static_cast<bool(Scene::*)() const>(&Scene::getPhysDrawDebug)), "getPhysDrawDebug");
				chai.add(chaiscript::fun(static_cast<void(Scene::*)(bool)>(&Scene::setPhysDrawDebug)), "setPhysDrawDebug");
				chai.add(chaiscript::fun(static_cast<FSELightWorld*(Scene::*)() const>(&Scene::getLightWorld)), "getLightWorld");
				chai.add(chaiscript::fun(static_cast<b2World*(Scene::*)()>(&Scene::getPhysWorld)), "getPhysWorld");
				chai.add(chaiscript::fun(static_cast<void(Scene::*)(std::shared_ptr<FSEObject>)>(&Scene::spawnFSEObject)), "spawnObject");
				chai.add(chaiscript::fun(([](Scene* scene)
				{			
					return scene->getFSEObjects();
				})), "getObjects");
				chai.add(chaiscript::fun(([](Scene* scene, int id)
				{
					for (auto& object : *scene->getFSEObjects())
					{
						if (object->getID() == id)
							return object;
					}
					return static_cast<std::shared_ptr<FSEObject>>(nullptr);
				})), "getObjectWithId");

				RegisterFSEObjects(chai);
				RegisterLights(chai);
		}


		void FSEChaiLib::RegisterFSEObjects(chaiscript::ChaiScript& chai)
		{
			chai.add(chaiscript::vector_conversion<std::vector<std::shared_ptr<FSEObject>>>());
			chai.add(chaiscript::bootstrap::standard_library::vector_type<std::vector<std::shared_ptr<FSEObject>>>("ObjectList"));

			RegisterChaiUserTypeFromRTTR<FSEObject>(chai);
			chai.add(chaiscript::fun(static_cast<int (FSEObject::*)() const>(&FSEObject::getID)), "getID");
			chai.add(chaiscript::fun(static_cast<int (FSEObject::*)() const>(&FSEObject::getZOrder)), "getZOrder");
			chai.add(chaiscript::fun(static_cast<void(FSEObject::*)(int)>(&FSEObject::setZOrder)), "setZOrder");
			chai.add(chaiscript::fun(static_cast<sf::Vector2f(FSEObject::*)()>(&FSEObject::getPosition)), "getPosition");
			chai.add(chaiscript::fun(static_cast<void(FSEObject::*)(sf::Vector2f position)>(&FSEObject::setPosition)),
				"setPosition");
			chai.add(chaiscript::fun([](FSEObject* object, float x, float y)
			{
				object->setPosition({ x, y });
			}), "setPosition");
			chai.add(chaiscript::fun(static_cast<sf::FloatRect(FSEObject::*)() const>(&FSEObject::GetAABBs)),
				"getAABBs");
			chai.add(chaiscript::fun(static_cast<bool(FSEObject::*)()>(&FSEObject::destroy)),
				"destroy");
			chai.add(chaiscript::fun(static_cast<Scene*(FSEObject::*)() const>(&FSEObject::getScene)),
				"getScene");
			chai.add(chaiscript::fun(([](const FSEObject* object) {
				return object->get_type().get_name().to_string();
			})), "getTypeName");
			chai.add(chaiscript::fun(([](const FSEObject* object) {
				return object->get_type().get_name().to_string();
			})), "type_name");

			RegisterChaiUserTypeFromRTTR<KillVolume>(chai);
			chai.add(chaiscript::base_class<fse::FSEObject, KillVolume>());
			chai.add(chaiscript::constructor<KillVolume()>(), "KillVolume");
			chai.add(chaiscript::fun(static_cast<const sf::Vector2f&(KillVolume::*)() const>(&KillVolume::getSize)), "getSize");
			chai.add(chaiscript::fun(static_cast<void(KillVolume::*)(const sf::Vector2f& size)>(&KillVolume::setSize)),
				"setSize");

			RegisterChaiUserTypeFromRTTR<FSELightWorld>(chai);
			chai.add(chaiscript::base_class<fse::FSEObject, FSELightWorld>());
			chai.add(chaiscript::fun((&FSELightWorld::lighting_)), "lighting");
			chai.add(chaiscript::fun(static_cast<bool(FSELightWorld::*)() const>(&FSELightWorld::getBloom)), "getBloom");
			chai.add(chaiscript::fun(static_cast<void(FSELightWorld::*)(bool)>(&FSELightWorld::setBloom)), "setBloom");
			chai.add(chaiscript::fun(static_cast<sf::Color(FSELightWorld::*)() const>(&FSELightWorld::getAmbientColor)), "getAmbientColor");
			chai.add(chaiscript::fun(static_cast<void(FSELightWorld::*)(const sf::Color color) const>(&FSELightWorld::setAmbientColor)), "setAmbientColor");
			chai.add(chaiscript::fun([](const FSELightWorld& lightWorld)
			{
				const std::vector<Light*> result = lightWorld.lights_;
				return result;
			}), "getLights");


			RegisterChaiUserTypeFromRTTR<FPSCounter>(chai);
			chai.add(chaiscript::base_class<fse::FSEObject, FPSCounter>());
			chai.add(chaiscript::fun((&FPSCounter::detailed_view_)), "detailed");

			RegisterChaiUserTypeFromRTTR<Timer>(chai);
			chai.add(chaiscript::base_class<fse::FSEObject, Timer>());
			chai.add(chaiscript::fun((&Timer::active_)), "active");
			chai.add(chaiscript::fun((&Timer::single_shot_)), "singleShot");
			chai.add(chaiscript::fun((&Timer::interval_)), "interval");
			chai.add(chaiscript::fun(((&Timer::stop))), "stop");
			chai.add(chaiscript::fun(([](Timer* timer, std::function<void()> slot) { timer->start(slot); })), "start");
			chai.add(chaiscript::fun(([](Scene* scene, int msecs, std::function<void()> slot) { Timer::singleShot(scene, msecs, slot); })),
				"startSingleShotTimer");
		}

		void FSEChaiLib::RegisterLights(chaiscript::ChaiScript& chai)
		{
			chai.add(chaiscript::vector_conversion<std::vector<Light*>>());
			chai.add(chaiscript::bootstrap::standard_library::vector_type<std::vector<Light*>>("LightList"));

			RegisterChaiUserTypeFromRTTR<fse::Light>(chai);	
			chai.add(chaiscript::fun(static_cast<void (Light::*)(const sf::Vector2f) const>(&Light::setPosition)), "setPosition");
			chai.add(chaiscript::fun(static_cast<sf::Vector2f(Light::*)() const>(&Light::getPosition)), "getPosition");

			chai.add(chaiscript::fun(static_cast<void (Light::*)(const float)>(&Light::setZPosition)), "setZPosition");
			chai.add(chaiscript::fun(static_cast<float(Light::*)() const>(&Light::getZPosition)), "getZPosition");

			chai.add(chaiscript::fun(static_cast<void (Light::*)(const sf::Vector2f) const>(&Light::setScale)), "setScale");
			chai.add(chaiscript::fun(static_cast<sf::Vector2f(Light::*)() const>(&Light::getScale)), "getScale");
			chai.add(chaiscript::fun(static_cast<void (Light::*)(const sf::Color) const>(&Light::setColor)), "setColor");
			chai.add(chaiscript::fun(static_cast<sf::Color(Light::*)() const>(&Light::getColor)), "getColor");

			chai.add(chaiscript::fun(static_cast<void (Light::*)(float) const>(&Light::setRotation)), "setRotation");
			chai.add(chaiscript::fun(static_cast<float(Light::*)() const>(&Light::getRotation)), "getRotation");
			chai.add(chaiscript::fun(static_cast<void(Light::*)(float) const>(&Light::rotate)), "rotate");

			chai.add(chaiscript::fun(static_cast<void (Light::*)(bool) const>(&Light::setTurnedOn)), "setTurnedOn");
			chai.add(chaiscript::fun(static_cast<bool(Light::*)() const>(&Light::isTurnedOn)), "isTurnedOn");
			chai.add(chaiscript::fun(static_cast<void(Light::*)() const>(&Light::toggleTurnedOn)), "toggle");

			chai.add(chaiscript::constructor<Light()>(), "Light");
			chai.add(chaiscript::constructor<Light(const Light&)>(), "Light");
			chai.add(chaiscript::constructor<Light(Scene*, const sf::Vector2f&, const std::string&, bool)>(), "Light");
			chai.add(chaiscript::fun(static_cast<Light&(Light::*)(const Light&)>(&Light::operator=)), "=");



			RegisterChaiUserTypeFromRTTR<PointLight>(chai);
			chai.add(chaiscript::base_class<fse::Light, PointLight>());
			chai.add(chaiscript::fun(static_cast<void (PointLight::*)(float) const>(&PointLight::setRadius)), "setRadius");
			chai.add(chaiscript::fun(static_cast<float(PointLight::*)() const>(&PointLight::getRadius)), "getRadius");

			chai.add(chaiscript::constructor<PointLight()>(), "PointLight");
			chai.add(chaiscript::constructor<PointLight(Scene*, const sf::Vector2f&)>(), "PointLight");



			RegisterChaiUserTypeFromRTTR<SpotLight>(chai);
			chai.add(chaiscript::base_class<fse::Light, SpotLight>());
			chai.add(chaiscript::fun(static_cast<void (SpotLight::*)(float)>(&SpotLight::setLenght)), "setLenght");
			chai.add(chaiscript::fun(static_cast<float(SpotLight::*)() const>(&SpotLight::getLenght)), "getLenght");
			chai.add(chaiscript::fun(static_cast<void (SpotLight::*)(float)>(&SpotLight::setAngle)), "setAngle");
			chai.add(chaiscript::fun(static_cast<float(SpotLight::*)() const>(&SpotLight::getAngle)), "getAngle");

			chai.add(chaiscript::constructor<SpotLight()>(), "SpotLight");
			chai.add(chaiscript::constructor<SpotLight(Scene*, const sf::Vector2f&)>(), "SpotLight");
		}
	}
}
