#include "FSELightWorld.h"
#include "../Application.h"

#include <rttr/registration>

#include <SFML/Graphics.hpp>

namespace fse
{
	FSELightWorld::FSELightWorld(Scene* scene) : FSELightWorld(scene, sf::Vector2f(0, 0))
	{

	}

	FSELightWorld::FSELightWorld(Scene* scene, const sf::Vector2f& spawnPos) : FSEObject(scene, spawnPos)
	{
		setZOrder(255);
		light_system_ = std::make_unique<ltbl::LightSystem>(normal_texture_, specular_texture_, true);
		bloom_texture_.create(1920, 1080);
		b_texture_.create(1920, 1080);

		bloom_shader_.loadFromMemory(bloom_shader_str_, sf::Shader::Fragment);
		gauss_blur_shader_.loadFromMemory(gauss_blur_shader_str_, sf::Shader::Fragment);
		blend_shader_.loadFromMemory(blend_shader_str_, sf::Shader::Fragment);
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

			if (bloom_)
			{
				bloom_texture_.clear(sf::Color::Black);

				sf::View view = scene_->getRenderTarget()->getView();

				if (sf::RenderWindow* window = dynamic_cast<sf::RenderWindow*>(&target))
				{

					b_texture_.update(*window);

					sf::Sprite sprite = sf::Sprite(b_texture_);
					//sprite.setPosition(view.getCenter() - view.getSize() / 2.f);

					bloom_shader_.setUniform("currTex", sf::Shader::CurrentTexture);

					bloom_texture_.draw(sprite, &bloom_shader_);
					bloom_texture_.display();

					gauss_blur_shader_.setUniform("currTex", sf::Shader::CurrentTexture);
					gauss_blur_shader_.setUniform("texSize", view.getSize());

					bool horizontal = false;
					for (int i = 0; i < 4; i++)
					{
						horizontal = !horizontal;
						gauss_blur_shader_.setUniform("horizontal", horizontal);
						bloom_texture_.draw(sf::Sprite(bloom_texture_.getTexture()), &gauss_blur_shader_);
						bloom_texture_.display();
					}


					sprite = sf::Sprite(bloom_texture_.getTexture());
					sprite.setPosition(view.getCenter() - view.getSize() / 2.f);

					//blend_shader_.setUniform("exposure", exposure_);
					//blend_shader_.setUniform("bloomBlur", sf::Shader::CurrentTexture);
					//blend_shader_.setUniform("scene", b_texture_);


					//window->draw(sprite, &blend_shader_);
					//window->draw(sprite);

					window->draw(sprite, sf::BlendAdd);

				}
				else if (false)
				{
					
				}

			}

		}
	}

	void FSELightWorld::spawned()
	{
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
			if (bloom_texture_.getSize() != scene_->getRenderTarget()->getSize())
			{
				bloom_texture_.create(scene_->getRenderTarget()->getSize().x, scene_->getRenderTarget()->getSize().y);
				b_texture_.create(scene_->getRenderTarget()->getSize().x, scene_->getRenderTarget()->getSize().y);
			}

			sf::View view = scene_->getRenderTarget()->getView();
			normal_texture_.setView(view);
			specular_texture_.setView(view);
			//bloom_texture_.setView(view);
			normal_texture_.clear(sf::Color(128u, 128u, 255u));
			specular_texture_.clear(sf::Color::Black);
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
	.property("bloom_", &FSELightWorld::bloom_)
	.property("exposure_", &FSELightWorld::exposure_)
	.property("ambient_color_", &FSELightWorld::getAmbientColor, &FSELightWorld::setAmbientColor)
	;
}
