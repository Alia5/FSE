#include "FSELightWorld.h"
#include "../Application.h"

#include <rttr/registration>

namespace fse
{
	FSELightWorld::FSELightWorld(Scene* scene) : FSELightWorld(scene, sf::Vector2f(0, 0))
	{

	}

	FSELightWorld::FSELightWorld(Scene* scene, const sf::Vector2f& spawnPos) : FSEObject(scene, spawnPos)
	{
		setZOrder(255);
		light_system_ = std::make_unique<ltbl::LightSystem>(normal_texture_, specular_texture_, true);
	}

	FSELightWorld::~FSELightWorld()
	{
		if (sun_ != nullptr)
			light_system_->removeLight(sun_);
	}

	void FSELightWorld::update(float deltaTime)
	{

	}

	void FSELightWorld::draw(sf::RenderTarget& target)
	{
		if (lighting_)
		{
			normal_texture_.display();
			specular_texture_.display();
			light_system_->render(target);
		}
	}

	void FSELightWorld::spawned()
	{
	}

	bool FSELightWorld::destroy()
	{
		return false;
	}

	void FSELightWorld::init(sf::RenderTarget* target) const
	{
		light_system_->create({ -1000.f, -1000.f, 2000.f, 2000.f }, target->getSize());
		//sun_ = light_system_->createLightDirectionEmission();
		//sun_->setColor(sf::Color::Black);
	}

	bool FSELightWorld::getLighting() const
	{
		return lighting_;
	}

	void FSELightWorld::setLighting(bool lighting)
	{
		lighting_ = lighting;
	}

	void FSELightWorld::updateView()
	{
		if (lighting_)
		{
			sf::View view = scene_->getRenderTarget()->getView();
			normal_texture_.clear(sf::Color(128u, 128u, 255u));
			specular_texture_.clear(sf::Color::Black);
			normal_texture_.setView(view);
			specular_texture_.setView(view);
		}
	}

	sf::Color FSELightWorld::getAmbientColor() const
	{
		return light_system_->getAmbientColor();
	}

	void FSELightWorld::setAmbientColor(const sf::Color color) const
	{
		light_system_->setAmbientColor(color);
	}

	ltbl::LightDirectionEmission* FSELightWorld::getSun()
	{
		if (sun_ == nullptr)
			sun_ = light_system_->createLightDirectionEmission();
		return sun_;
	}

	ltbl::LightSystem* FSELightWorld::getLightSystem() const
	{
		return light_system_.get();
	}

	sf::RenderTarget* FSELightWorld::getNormalTarget()
	{
		return &normal_texture_;
	}

	sf::RenderTarget* FSELightWorld::getSpecularTarget()
	{
		return &specular_texture_;
	}
}

RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace fse;

	registration::class_<FSELightWorld>("fse::FSELightWorld")
	.property("lighting_", &FSELightWorld::lighting_)
	.property("ambient_color_", &FSELightWorld::getAmbientColor, &FSELightWorld::setAmbientColor)
	;
}
