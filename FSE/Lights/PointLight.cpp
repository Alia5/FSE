#include "PointLight.h"
#include "../Application.h"

#include <rttr/registration>

namespace fse
{
	PointLight::PointLight() : Light()
	{
		
	}

	PointLight::PointLight(Scene* scene, const sf::Vector2f& spawnPos) : Light(scene, spawnPos, "lights/pointLightTexture.png", true)
	{
		light_->setScale((FSE_PIXELS_PER_METER / light_texture_->getSize().x), (FSE_PIXELS_PER_METER / light_texture_->getSize().x));
	}

	PointLight::~PointLight()
	{
		
	}

	void PointLight::setRadius(float radius) const
	{
		light_->setScale(sf::Vector2f(radius,radius) * (FSE_PIXELS_PER_METER / light_texture_->getSize().x));
	}

	float PointLight::getRadius() const
	{
		return light_->getScale().x / (FSE_PIXELS_PER_METER / light_texture_->getSize().x);
	}

	FSE_CHAI_REGISTER(PointLight)
	{
		RegisterChaiUserTypeFromRTTR<PointLight>(chai);
		chai.add(chaiscript::base_class<fse::Light, PointLight>());
		chai.add(chaiscript::fun(static_cast<void (PointLight::*)(float) const>(&PointLight::setRadius)), "setRadius");
		chai.add(chaiscript::fun(static_cast<float(PointLight::*)() const>(&PointLight::getRadius)), "getRadius");

		chai.add(chaiscript::constructor<PointLight()>(), "PointLight");
		chai.add(chaiscript::constructor<PointLight(Scene*, const sf::Vector2f&)>(), "PointLight");
	}

}

RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace fse;

registration::class_<PointLight>("fse::PointLight")
.constructor([](Scene* scene)
{
	return PointLight(scene, sf::Vector2f(0, 0));
})
(
	parameter_names("scene")			
)
.property("radius_", &PointLight::getRadius, &PointLight::setRadius)
;
}
