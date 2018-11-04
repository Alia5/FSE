#include "BrickBackground.h"

#include <FSE/Application.h>
#include <FSE/Lights/FSELightWorld.h>
#include <FSE/FMath.h>
#include <random>

BrickBackground::BrickBackground() : BrickBackground({ 0.f, 0.f })
{

}

BrickBackground::BrickBackground(const sf::Vector2f& spawnPos) : BrickBackground( spawnPos, { 1.f, 1.f })
{
}

BrickBackground::BrickBackground(const sf::Vector2f& spawnPos, const sf::Vector2f& size) : FSEObject(spawnPos)
{
	//truncate size to whole meters
	size_.x = std::trunc(size.x);
	size_.y = std::trunc(size.y);
	if (size_.x < 1.f)
		size_.x = 1.f;
	if (size_.y < 1.f)
		size_.y = 1.f;

	setZOrder(15);
}


void BrickBackground::draw(sf::RenderTarget& target)
{
	sprite_.render(target);
}

void BrickBackground::drawNormals(sf::RenderTarget& target)
{
	sprite_.renderNormals(target);
}

void BrickBackground::drawSpecular(sf::RenderTarget& target)
{
	sprite_.renderSpecular(target);
}

void BrickBackground::spawned()
{
	texture_ = scene_->getApplication()->getAssetLoader().getTexture("Brickwall/BrickSmallPatternsDarkTex.png");
	normal_texture_ = scene_->getApplication()->getAssetLoader().getTexture("Brickwall/BrickSmallPatterns_normal.png");
	specular_texture = scene_->getApplication()->getAssetLoader().getTexture("Brickwall/BrickSmallPatterns_specular.png");

	texture_->setRepeated(true);
	normal_texture_->setRepeated(true);
	specular_texture->setRepeated(true);

	texture_->setSmooth(true);
	normal_texture_->setSmooth(true);
	specular_texture->setSmooth(true);

	sprite_ = ltbl::Sprite(scene_->getLightWorld()->getLightSystem());
	sprite_.setTexture(*texture_);
	sprite_.setNormalsTexture(*normal_texture_);
	sprite_.setSpecularTexture(*specular_texture);

	sprite_.setTextureRect(sf::IntRect(0, 0,
		static_cast<int>(size_.x * FSE_PIXELS_PER_METER),
		static_cast<int>(size_.y * FSE_PIXELS_PER_METER)));

	sprite_.setOrigin(size_ / 2.f);
	sprite_.setPosition(position_ * FSE_PIXELS_PER_METER);
}

void BrickBackground::setPosition(const sf::Vector2f position)
{
	position_ = position;
	sprite_.setPosition(position_ * FSE_PIXELS_PER_METER);
}

sf::FloatRect BrickBackground::GetAABBs() const
{
	return sprite_.getGlobalBounds();
}

const sf::Vector2f& BrickBackground::getSize()
{
	return size_;
}

void BrickBackground::setSize(const sf::Vector2f& size)
{
	if (size_.x == std::trunc(size.x)
		&& size_.y == std::trunc(size.y))
		return;
	//truncate size to whole meters
	size_.x = std::trunc(size.x);
	size_.y = std::trunc(size.y);
	if (size_.x < 1.f)
		size_.x = 1.f;
	if (size_.y < 1.f)
		size_.y = 1.f;

	//reset sprites texture rect
	sprite_.setTextureRect(sf::IntRect(0, 0, size_.x * FSE_PIXELS_PER_METER, size_.y * FSE_PIXELS_PER_METER));
	sprite_.setOrigin(size_ / 2.f);
	sprite_.setPosition(position_ * FSE_PIXELS_PER_METER);
}

float BrickBackground::getRotation() const
{
	return sprite_.getRotation();
}

void BrickBackground::setRotation(float rot)
{
	sprite_.setRotation(rot);
}


#include <rttr/registration>
RTTR_REGISTRATION
{
	using namespace rttr;

	registration::class_<BrickBackground>("BrickBackground")
	.constructor<>()
	(
		policy::ctor::as_std_shared_ptr
	)
	.constructor<const sf::Vector2f&>()
	(
		policy::ctor::as_std_shared_ptr,
		parameter_names("spawn position")
	)
	.constructor<const sf::Vector2f&, const sf::Vector2f&>()
	(
		policy::ctor::as_std_shared_ptr,
		parameter_names("spawn position", "size")
	)
	.property("size_", &BrickBackground::getSize, &BrickBackground::setSize)
	(
		metadata("CTOR_ARG", "size")
	)
	.property("rotation_", &BrickBackground::getRotation, &BrickBackground::setRotation)
	;
}