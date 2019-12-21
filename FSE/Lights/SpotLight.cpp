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

	FSE_V8_REGISTER(SpotLight)
	{
		v8::HandleScope handle_scope(isolate);
		v8pp::class_<SpotLight> SpotLight_class(isolate);
		SpotLight_class.auto_wrap_objects(true);
		SpotLight_class.inherit<Light>();
		SpotLight_class.var("length", &SpotLight::lenght_);
		SpotLight_class.var("angle", &SpotLight::angle_);


		SpotLight_class.ctor<void>(
			[](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				return new SpotLight();
			});
		SpotLight_class.ctor<Scene*>(
			[](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				return new SpotLight(v8pp::from_v8<Scene*>(args.GetIsolate(), args[0]), { 0,0 });
			});
		SpotLight_class.ctor<Scene*, const sf::Vector2f&>(
			[](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				return new SpotLight(v8pp::from_v8<Scene*>(args.GetIsolate(), args[0]),
					v8pp::from_v8<const sf::Vector2f&>(args.GetIsolate(), args[1]));
			});

		module.class_("SpotLight", SpotLight_class);
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