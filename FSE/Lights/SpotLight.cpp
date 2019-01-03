#include "SpotLight.h"
#include "../Application.h"

#include <rttr/registration.h>

namespace fse
{
	SpotLight::SpotLight()
	{
	}

	SpotLight::SpotLight(Scene* scene, const sf::Vector2f& spawnPos) : Light(scene, spawnPos, "lights/spotLightTexture.png", true)
	{
		light_->setOrigin(sf::Vector2f(light_texture_->getSize().x / 2.f, 0.f));
		lenght_ = light_texture_->getSize().y * scene_->getMetersPerPixel();
	}


	SpotLight::~SpotLight()
	{
	}

	void SpotLight::setLenght(float lenght)
	{
		lenght_ = lenght;
		light_->setScale(light_->getScale().x * lenght * 5.12f * scene_->getMetersPerPixel(), lenght * 5.12f * scene_->getMetersPerPixel());
	}

	float SpotLight::getLenght() const
	{
		return lenght_;
	}

	void SpotLight::setAngle(float angle)
	{
		angle_ = std::min(angle, 179.9f);
		light_->setScale(light_->getScale().x * light_->getScale().y * std::tan((angle_ * 3.14159265f / 180.0f) / 2.f), light_->getScale().y);

	}

	float SpotLight::getAngle() const
	{
		return angle_;
	}

	FSE_CHAI_REGISTER(SpotLight)
	{
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

RTTR_REGISTRATION
{
	using namespace rttr;
using namespace fse;

registration::class_<SpotLight>("fse::SpotLight")
.constructor([](Scene* scene)
{
	return SpotLight(scene, sf::Vector2f(0, 0));
})
(
	parameter_names("scene")
)
.property("lenght_", &SpotLight::getLenght, &SpotLight::setLenght)
.property("angle_", &SpotLight::getAngle, &SpotLight::setAngle)
;
}