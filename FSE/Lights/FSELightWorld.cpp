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
		bloom_texture_.create(1, 1);
		bloom_shader_.loadFromMemory(bloom_shader_str_, sf::Shader::Fragment);
		gauss_blur_shader_.loadFromMemory(gauss_blur_shader_str_, sf::Shader::Fragment);

#ifdef ANDROID
        lighting_ = false;
#endif
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
				if (sf::RenderTexture* r_texture = dynamic_cast<sf::RenderTexture*>(&target))
				{
					//r_texture->display();
					bloom_texture_.clear();
					sf::Sprite sprite = sf::Sprite(bloom_texture_.getTexture());
					auto view = target.getView();

					bloom_shader_.setUniform("currTex", r_texture->getTexture());
					bloom_shader_.setUniform("lightCompTex", light_system_->getLightCompTexture().getTexture());
					bloom_shader_.setUniform("specCompTex", light_system_->getSpecCompTexture().getTexture());

					bloom_texture_.draw(sprite, &bloom_shader_);

					gauss_blur_shader_.setUniform("currTex", bloom_texture_.getTexture());
					gauss_blur_shader_.setUniform("texSize", sf::Vector2f(target.getSize().x, target.getSize().y));
					bool horizontal = false;
					for (int i = 0; i < 4; i++)
					{
						horizontal = !horizontal;
						gauss_blur_shader_.setUniform("horizontal", horizontal);
						bloom_texture_.draw(sf::Sprite(bloom_texture_.getTexture()), &gauss_blur_shader_);
					}

					bloom_texture_.display();

					auto sz = view.getSize();
					sprite.setPosition(view.getCenter() - view.getSize() / 2.f);
					sprite.setScale(sz.x / target.getSize().x, sz.y / target.getSize().y);
					target.draw(sprite, sf::BlendAdd);

				}
			}
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
		light_system_->create({ -1000.f, -1000.f, 1000.f, 1000.f }, target->getSize());
		//sun_ = light_system_->createLightDirectionEmission();
		//sun_->setColor(sf::Color::Black);
	}

	bool FSELightWorld::getLighting() const
	{
		return lighting_;
	}

	void FSELightWorld::setLighting(bool lighting)
	{
#ifdef ANDROID
        lighting_ = false;
        return;
#endif
		lighting_ = lighting;
	}

	bool FSELightWorld::getBloom() const
	{
		return bloom_;
	}

	void FSELightWorld::setBloom(bool bloom)
	{
		if (sf::RenderTexture* rTexture = dynamic_cast<sf::RenderTexture*>(scene_->getRenderTarget()))
		{
			bloom_ = bloom;
		} 
		else
		{
			std::cout << "Cannot use bloom directly on Window!\n";
			bloom_ = false;
		}
	}

	void FSELightWorld::updateView()
	{
		if (lighting_)
		{
			if (bloom_texture_.getSize() != scene_->getRenderTarget()->getSize())
			{
				bloom_texture_.create(scene_->getRenderTarget()->getSize().x, scene_->getRenderTarget()->getSize().y);
			}

			sf::View view = scene_->getRenderTarget()->getView();
			normal_texture_.setView(view);
			specular_texture_.setView(view);
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
	(
		metadata("SERIALIZE_NO_RECRATE", true)
	)
	.property("lighting_", &FSELightWorld::lighting_)
	.property("bloom_", &FSELightWorld::getBloom, &FSELightWorld::setBloom)
	.property("ambient_color_", &FSELightWorld::getAmbientColor, &FSELightWorld::setAmbientColor)
	;
}
