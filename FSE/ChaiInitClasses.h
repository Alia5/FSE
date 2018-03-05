#include <chaiscript/chaiscript.hpp>

namespace fse
{
	namespace priv
	{
		struct ChaiRegister
		{
			static  void registerFSETypes(chaiscript::ChaiScript& chai)
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

				chai.add(chaiscript::user_type<FSEObject>(), "FSEObject_NATIVE");
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
			}
		};
	}
}


//
//registration::class_<sf::Color>("sf::Color")
//.property("r", &sf::Color::r)
//.property("g", &sf::Color::g)
//.property("b", &sf::Color::b)
//.property("a", &sf::Color::a)
//;
//
//}