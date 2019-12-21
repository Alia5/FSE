#include "Light.h"
#include "../Application.h"
#include <rttr/registration>


namespace fse
{

	Light::Light()
	{
	
	}

	Light::Light(const Light& other) : scene_(other.scene_), light_texture_(other.light_texture_)
	{
		light_ = scene_->getLightWorld()->getLightSystem()->createLightPointEmission();
		*light_ = *other.light_;
	}

	Light::Light(Scene* scene, const sf::Vector2f& spawnPos, const std::string& path, bool smooth) : scene_(scene)
	{
		light_texture_ = scene->getApplication()->getAssetLoader().getTexture(path);
		light_texture_->setSmooth(smooth);

		light_ = scene->getLightWorld()->getLightSystem()->createLightPointEmission();
		light_->setOrigin(sf::Vector2f(light_texture_->getSize().x * 0.5f, light_texture_->getSize().y * 0.5f));
		light_->setTexture(*light_texture_);
		light_->setColor(sf::Color::White);
		light_->setPosition(spawnPos * scene_->getPixelsPerMeter());

		scene_->getLightWorld()->registerLight(this);
	}

	Light::~Light()
	{
		if (scene_ != nullptr)
		{
			if (light_ != nullptr && scene_->getLightWorld() != nullptr)
			{
				scene_->getLightWorld()->getLightSystem()->removeLight(light_);
			}
			scene_->getLightWorld()->unregisterLight(this);
		}

	}

	void Light::setPosition(const sf::Vector2f pos) const
	{
		light_->setPosition(pos * scene_->getPixelsPerMeter());
	}

	void Light::setColor(const sf::Color color) const
	{
		light_->setColor(color);
	}

	void Light::setScale(const sf::Vector2f scale) const
	{
		light_->setScale(scale);
	}

	sf::Vector2f Light::getPosition() const
	{
		return light_->getPosition() * scene_->getMetersPerPixel();
	}

	sf::Color Light::getColor() const
	{
		return light_->getColor();
	}

	sf::Vector2f Light::getScale() const
	{
		return light_->getScale();
	}

	void Light::rotate(float angle) const
	{
		light_->rotate(angle);
	}

	void Light::setRotation(float angle) const
	{
		light_->setRotation(angle);
	}

	float Light::getRotation() const
	{
		return light_->getRotation();
	}

	void Light::setTurnedOn(bool on) const
	{
		light_->setTurnedOn(on);
	}

	bool Light::isTurnedOn() const
	{
		return light_->isTurnedOn();
	}

	void Light::toggleTurnedOn() const
	{
		light_->toggleTurnedOn();
	}

	float Light::getZPosition() const
	{
		return light_->getZPosition();
	}

	void Light::setZPosition(const float zPos)
	{
		light_->setZPosition(zPos);
	}

	Light& Light::operator=(const Light& other)
	{
		if (&other == this)
			return *this;

		Light tmp(other);
		std::swap(light_, tmp.light_);
		std::swap(scene_, tmp.scene_);
		std::swap(light_texture_, tmp.light_texture_);

		scene_->getLightWorld()->unregisterLight(&other);
		scene_->getLightWorld()->registerLight(this);

		return *this;
	}

	FSE_V8_REGISTER_BASE(Light)
	{
		v8::HandleScope handle_scope(isolate);
		v8pp::class_<Light> Light_class(isolate);
		Light_class.auto_wrap_objects(true);
		Light_class.function("setPosition", static_cast<void (Light::*)(const sf::Vector2f) const>(&Light::setPosition));
		Light_class.function("getPosition", static_cast<sf::Vector2f(Light::*)() const>(&Light::getPosition));
		Light_class.property("position", &Light::getPosition, &Light::setPosition);

		Light_class.function("setZPosition", static_cast<void (Light::*)(const float)>(&Light::setZPosition));
		Light_class.function("getZPosition", static_cast<float(Light::*)() const>(&Light::getZPosition));
		Light_class.property("zPosition", &Light::getZPosition, &Light::setZPosition);


		Light_class.function("setScale", static_cast<void (Light::*)(const sf::Vector2f) const>(&Light::setScale));
		Light_class.function("getScale", static_cast<sf::Vector2f(Light::*)() const>(&Light::getScale));
		Light_class.property("scale", &Light::getScale, &Light::setScale);

		
		Light_class.function("setColor", static_cast<void (Light::*)(const sf::Color) const>(&Light::setColor));
		Light_class.function("getColor", static_cast<sf::Color(Light::*)() const>(&Light::getColor));
		Light_class.property("color", &Light::getColor, &Light::setColor);

		Light_class.function("setRotation", static_cast<void (Light::*)(float) const>(&Light::setRotation));
		Light_class.function("getRotation", static_cast<float(Light::*)() const>(&Light::getRotation));
		Light_class.function("rotate", static_cast<void(Light::*)(float) const>(&Light::rotate));
		Light_class.property("rotation", &Light::getRotation, &Light::setRotation);

		Light_class.function("setTurnedOn", static_cast<void (Light::*)(bool) const>(&Light::setTurnedOn));
		Light_class.function("isTurnedOn", static_cast<bool(Light::*)() const>(&Light::isTurnedOn));
		Light_class.function("toggle", static_cast<void(Light::*)() const>(&Light::toggleTurnedOn));
		Light_class.property("turnedOn", &Light::isTurnedOn, &Light::setTurnedOn);

		Light_class.ctor<void>(
			[](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				return new Light();
			});
		Light_class.ctor<const Light&>(
			[](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				return new Light(v8pp::from_v8<Light>(args.GetIsolate(), args[0]));
			});
		Light_class.ctor<Scene*, const sf::Vector2f&, const std::string&, bool>(
			[](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				return new Light(v8pp::from_v8<Scene*>(args.GetIsolate(), args[0]),
					v8pp::from_v8<const sf::Vector2f&>(args.GetIsolate(), args[1]),
					v8pp::from_v8<const std::string&>(args.GetIsolate(), args[2]),
					v8pp::from_v8<bool>(args.GetIsolate(), args[3]));
			});
		
		module.class_("Light", Light_class);

		
		//RegisterJSUserTypeFromRTTR<fse::Light>(isolate);
		//chai.add(chaiscript::fun(static_cast<void (Light::*)(const sf::Vector2f) const>(&Light::setPosition)), "setPosition");
		//chai.add(chaiscript::fun(static_cast<sf::Vector2f(Light::*)() const>(&Light::getPosition)), "getPosition");

		//chai.add(chaiscript::fun(static_cast<void (Light::*)(const float)>(&Light::setZPosition)), "setZPosition");
		//chai.add(chaiscript::fun(static_cast<float(Light::*)() const>(&Light::getZPosition)), "getZPosition");

		//chai.add(chaiscript::fun(static_cast<void (Light::*)(const sf::Vector2f) const>(&Light::setScale)), "setScale");
		//chai.add(chaiscript::fun(static_cast<sf::Vector2f(Light::*)() const>(&Light::getScale)), "getScale");
		//chai.add(chaiscript::fun(static_cast<void (Light::*)(const sf::Color) const>(&Light::setColor)), "setColor");
		//chai.add(chaiscript::fun(static_cast<sf::Color(Light::*)() const>(&Light::getColor)), "getColor");

		//chai.add(chaiscript::fun(static_cast<void (Light::*)(float) const>(&Light::setRotation)), "setRotation");
		//chai.add(chaiscript::fun(static_cast<float(Light::*)() const>(&Light::getRotation)), "getRotation");
		//chai.add(chaiscript::fun(static_cast<void(Light::*)(float) const>(&Light::rotate)), "rotate");

		//chai.add(chaiscript::fun(static_cast<void (Light::*)(bool) const>(&Light::setTurnedOn)), "setTurnedOn");
		//chai.add(chaiscript::fun(static_cast<bool(Light::*)() const>(&Light::isTurnedOn)), "isTurnedOn");
		//chai.add(chaiscript::fun(static_cast<void(Light::*)() const>(&Light::toggleTurnedOn)), "toggle");

		//chai.add(chaiscript::constructor<Light()>(), "Light");
		//chai.add(chaiscript::constructor<Light(const Light&)>(), "Light");
		//chai.add(chaiscript::constructor<Light(Scene*, const sf::Vector2f&, const std::string&, bool)>(), "Light");
		//chai.add(chaiscript::fun(static_cast<Light&(Light::*)(const Light&)>(&Light::operator=)), "=");
	}

}

RTTR_REGISTRATION
{
	using namespace rttr;
using namespace fse;

registration::class_<Light>("fse::Light")
.property("position_", &Light::getPosition, &Light::setPosition)
.property("z_position_", &Light::getZPosition, &Light::setZPosition)
.property("scale_", &Light::getScale, &Light::setScale)
.property("turned_on_", &Light::isTurnedOn, &Light::setTurnedOn)
.property("rotation_", &Light::getRotation, &Light::setRotation)
.property("color_", &Light::getColor, &Light::setColor)
;

}
