#include <chaiscript/chaiscript.hpp>
#include "FSEObject/KillVolume.h"
#include "Lights/FSELightWorld.h"

namespace fse
{
	namespace priv
	{
		class FSEChaiRegister
		{
		public:
			static void registerFSETypes(chaiscript::ChaiScript& chai)
			{
				chai.add(chaiscript::user_type<sf::Vector2f>(), "Vector2f");
				chai.add(chaiscript::fun(&sf::Vector2f::x), "x");
				chai.add(chaiscript::fun(&sf::Vector2f::y), "y");
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

				chai.add(chaiscript::user_type<sf::Vector2i>(), "Vector2i");
				chai.add(chaiscript::fun(&sf::Vector2i::x), "x");
				chai.add(chaiscript::fun(&sf::Vector2i::y), "y");
				chai.add(chaiscript::constructor<sf::Vector2i(float, float)>(), "Vector2i");
				chai.add(chaiscript::constructor<sf::Vector2i(const sf::Vector2i&)>(), "Vector2i");
				chai.add(chaiscript::fun([](sf::Vector2i& lhs, const sf::Vector2i& rhs) { return lhs = rhs; }), "=");
				chai.add(chaiscript::fun([](sf::Vector2i& lhs, const sf::Vector2i& rhs) { return lhs + rhs; }), "+");
				chai.add(chaiscript::fun([](sf::Vector2i& lhs, const sf::Vector2i& rhs) { return lhs - rhs; }), "-");
				chai.add(chaiscript::fun([](const sf::Vector2i& lhs, const sf::Vector2i& rhs) { return lhs == rhs; }), "==");


				chai.add(chaiscript::user_type<sf::IntRect>(), "IntRect");
				chai.add(chaiscript::fun(&sf::IntRect::top), "top");
				chai.add(chaiscript::fun(&sf::IntRect::left), "left");
				chai.add(chaiscript::fun(&sf::IntRect::width), "width");
				chai.add(chaiscript::fun(&sf::IntRect::height), "height");
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
				chai.add(chaiscript::constructor<sf::FloatRect(int, int, int, int)>(), "FloatRect");
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
				chai.add(chaiscript::constructor<sf::Color(int, int, int, int)>(), "Color");
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
				

				chai.add(chaiscript::user_type<FSEObject>(), "FSEObject");
				chai.add(chaiscript::fun(static_cast<int (FSEObject::*)() const>(&FSEObject::getID)), "getID");
				chai.add(chaiscript::fun(static_cast<int (FSEObject::*)() const>(&FSEObject::getZOrder)), "getZOrder");
				chai.add(chaiscript::fun(static_cast<void(FSEObject::*)(int)>(&FSEObject::setZOrder)), "setZOrder");
				chai.add(chaiscript::fun(static_cast<sf::Vector2f(FSEObject::*)()>(&FSEObject::getPosition)), "getPosition");
				chai.add(chaiscript::fun(static_cast<void(FSEObject::*)(sf::Vector2f position)>(&FSEObject::setPosition)),
					"setPosition");
				chai.add(chaiscript::fun(static_cast<sf::FloatRect(FSEObject::*)() const>(&FSEObject::GetAABBs)),
					"getAABBs");
				chai.add(chaiscript::fun(static_cast<bool(FSEObject::*)()>(&FSEObject::destroy)),
					"destroy");
				chai.add(chaiscript::fun(static_cast<Scene*(FSEObject::*)() const>(&FSEObject::getScene)),
					"getScene");


				chai.add(chaiscript::user_type<KillVolume>(), "KillVolume");
				chai.add(chaiscript::base_class<fse::FSEObject, KillVolume>());
				chai.add(chaiscript::fun(static_cast<const sf::Vector2f&(KillVolume::*)() const>(&KillVolume::getSize)), "getSize");
				chai.add(chaiscript::fun(static_cast<void(KillVolume::*)(const sf::Vector2f& size)>(&KillVolume::setSize)),
					"setSize");


				chai.add(chaiscript::user_type<FSELightWorld>(), "FSELightWorld");
				chai.add(chaiscript::base_class<fse::FSEObject, FSELightWorld>());
				chai.add(chaiscript::fun((&FSELightWorld::lighting_)), "lighting");
				chai.add(chaiscript::fun(static_cast<bool(FSELightWorld::*)() const>(&FSELightWorld::getBloom)), "getBloom");
				chai.add(chaiscript::fun(static_cast<void(FSELightWorld::*)(bool)>(&FSELightWorld::setBloom)), "setBloom");
				chai.add(chaiscript::fun(static_cast<sf::Color(FSELightWorld::*)() const>(&FSELightWorld::getAmbientColor)), "getAmbientColor");
				chai.add(chaiscript::fun(static_cast<void(FSELightWorld::*)(const sf::Color color) const>(&FSELightWorld::setAmbientColor)), "setAmbientColor");

			}
		};
	}
}

