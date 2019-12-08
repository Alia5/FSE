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

	FSE_V8_REGISTER(Light)
	{
		RegisterJSUserTypeFromRTTR<fse::Light>(isolate);
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
