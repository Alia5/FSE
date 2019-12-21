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
		light_->setScale((scene_->getPixelsPerMeter() / light_texture_->getSize().x), (scene_->getPixelsPerMeter() / light_texture_->getSize().x));
	}

	PointLight::~PointLight()
	{
		
	}

	void PointLight::setRadius(float radius) const
	{
		light_->setScale(sf::Vector2f(radius,radius) * (scene_->getPixelsPerMeter() / light_texture_->getSize().x));
	}

	float PointLight::getRadius() const
	{
		return light_->getScale().x / (scene_->getPixelsPerMeter() / light_texture_->getSize().x);
	}

	FSE_V8_REGISTER(PointLight)
	{
		v8::HandleScope handle_scope(isolate);
		v8pp::class_<PointLight> PointLight_class(isolate);
		PointLight_class.auto_wrap_objects(true);
		PointLight_class.inherit<Light>();
		PointLight_class.function("setRadius", &PointLight::setRadius);
		PointLight_class.function("getRadius", &PointLight::getRadius);
		PointLight_class.property("radius", &PointLight::getRadius, &PointLight::setRadius);


		PointLight_class.ctor<void>(
			[](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				return new PointLight();
			});
		PointLight_class.ctor<Scene*>(
			[](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				return new PointLight(v8pp::from_v8<Scene*>(args.GetIsolate(), args[0]), {0,0});
			});
		PointLight_class.ctor<Scene*, const sf::Vector2f&>(
			[](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				return new PointLight(v8pp::from_v8<Scene*>(args.GetIsolate(), args[0]),
					v8pp::from_v8<const sf::Vector2f&>(args.GetIsolate(), args[1]));
			});

		module.class_("PointLight", PointLight_class);
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
