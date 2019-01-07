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

				chai.add(chaiscript::vector_conversion<std::vector<std::shared_ptr<FSEObject>>>());
				chai.add(chaiscript::bootstrap::standard_library::vector_type<std::vector<std::shared_ptr<FSEObject>>>("ObjectList"));


				chai.add(chaiscript::vector_conversion<std::vector<std::weak_ptr<FSEObject>>>());
				chai.add(chaiscript::bootstrap::standard_library::vector_type<std::vector<std::weak_ptr<FSEObject>>>("WeakObjectList"));

				chai.add(chaiscript::vector_conversion<std::vector<Light*>>());
				chai.add(chaiscript::bootstrap::standard_library::vector_type<std::vector<Light*>>("LightList"));

				chai_init::execute(chai);
		}

	}
}
