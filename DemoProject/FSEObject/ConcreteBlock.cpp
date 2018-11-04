#include "ConcreteBlock.h"

#include <FSE/Application.h>
#include <FSE/Lights/FSELightWorld.h>
#include <FSE/FMath.h>
#include <random>

ConcreteBlock::ConcreteBlock() : ConcreteBlock({ 0.f, 0.f })
{

}

ConcreteBlock::ConcreteBlock(const sf::Vector2f& spawnPos) : ConcreteBlock(spawnPos, { 1.f, 1.f })
{
}

ConcreteBlock::ConcreteBlock(const sf::Vector2f& spawnPos, const sf::Vector2f& size) : fse::FSEObject(spawnPos)
{
	//truncate size to whole meters
	size_.x = std::trunc(size.x);
	size_.y = std::trunc(size.y);
	if (size_.x < 1.f)
		size_.x = 1.f;
	if (size_.y < 1.f)
		size_.y = 1.f;

	setZOrder(32);
}

ConcreteBlock::~ConcreteBlock()
{
	if (scene_ != nullptr)
	{
		scene_->getLightWorld()->getLightSystem()->removeShape(light_shape_);
		scene_->getPhysWorld()->DestroyBody(phys_body_);
	}
}

void ConcreteBlock::update(float deltaTime)
{
}

void ConcreteBlock::draw(sf::RenderTarget& target)
{
	sprite_.render(target);
}

void ConcreteBlock::drawNormals(sf::RenderTarget& target)
{
	sprite_.renderNormals(target);
}

void ConcreteBlock::drawSpecular(sf::RenderTarget& target)
{
	sprite_.renderSpecular(target);
}

void ConcreteBlock::spawned()
{
	texture_ = scene_->getApplication()->getAssetLoader().getTexture("Concrete/Wall.png");
	normal_texture_ = scene_->getApplication()->getAssetLoader().getTexture("Concrete/Wall_normal.png");
	specular_texture = scene_->getApplication()->getAssetLoader().getTexture("Concrete/Wall_spec.png");

	texture_->setRepeated(true);
	normal_texture_->setRepeated(true);
	specular_texture->setRepeated(true);

	texture_->setSmooth(true);
	normal_texture_->setSmooth(true);
	specular_texture->setSmooth(true);


	b2BodyDef physBodyDef;

	physBodyDef.type = b2_staticBody;
	physBodyDef.position.Set(position_.x, position_.y);

	phys_body_ = getScene()->getPhysWorld()->CreateBody(&physBodyDef);
	phys_body_->SetUserData(this);

	b2PolygonShape box;
	box.SetAsBox(size_.x / 2.f, size_.y / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.friction = 1.f;
	fixtureDef.restitution = 0.001f;

	fixture_ = phys_body_->CreateFixture(&fixtureDef);

	//create light shape
	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(size_.x * FSE_PIXELS_PER_METER, size_.y * FSE_PIXELS_PER_METER));

	light_shape_ = getScene()->getLightWorld()->getLightSystem()->createLightShape(shape);
	light_shape_->setOrigin(shape.getSize() / 2.f);
	light_shape_->setPosition(position_ * FSE_PIXELS_PER_METER);
	light_shape_->setRenderLightOver(true);


	//prepare sprite
	sprite_ = ltbl::Sprite(scene_->getLightWorld()->getLightSystem());
	sprite_.setTexture(*texture_);
	sprite_.setNormalsTexture(*normal_texture_);
	sprite_.setSpecularTexture(*specular_texture);

	sprite_.setTextureRect(sf::IntRect(0, 0, size_.x * FSE_PIXELS_PER_METER + 25, size_.y * FSE_PIXELS_PER_METER));
	sprite_.setOrigin({ (shape.getSize().x + 25) / 2.f, shape.getSize().y / 2.f });
	sprite_.setPosition(position_ * FSE_PIXELS_PER_METER);
}

void ConcreteBlock::setPosition(const sf::Vector2f position)
{
	position_ = position;
	sprite_.setPosition(position_ * FSE_PIXELS_PER_METER);
	light_shape_->setPosition(position_ * FSE_PIXELS_PER_METER);
	phys_body_->SetTransform(fse::FMath::sfVec2fTob2Vec2(position), phys_body_->GetAngle());
}

sf::FloatRect ConcreteBlock::GetAABBs() const
{
	return sprite_.getGlobalBounds();
}

const sf::Vector2f& ConcreteBlock::getSize()
{
	return size_;
}

void ConcreteBlock::setSize(const sf::Vector2f& size)
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

	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(size_.x * FSE_PIXELS_PER_METER, size_.y * FSE_PIXELS_PER_METER));

	scene_->getLightWorld()->getLightSystem()->removeShape(light_shape_);
	light_shape_ = getScene()->getLightWorld()->getLightSystem()->createLightShape(shape);
	light_shape_->setOrigin(shape.getSize() / 2.f);
	light_shape_->setPosition(position_ * FSE_PIXELS_PER_METER);
	light_shape_->setRotation(sprite_.getRotation());
	light_shape_->setRenderLightOver(renderLightOver);
	light_shape_->setRenderLightOver(true);

	//reset sprites texture rect
	sprite_.setTextureRect(sf::IntRect(0, 0, size_.x * FSE_PIXELS_PER_METER + 25, size_.y * FSE_PIXELS_PER_METER ));
	sprite_.setOrigin({(shape.getSize().x + 25 ) / 2.f, shape.getSize().y / 2.f});
	sprite_.setPosition(position_ * FSE_PIXELS_PER_METER);

	//resize phys_shape
	auto* phys_shape = dynamic_cast<b2PolygonShape*>(fixture_->GetShape());
	phys_shape->SetAsBox(size_.x / 2.f, size_.y / 2.f);

}

float ConcreteBlock::getRotation() const
{
	if (phys_body_ == nullptr)
		return 0;
	return phys_body_->GetAngle() * FSE_RADTODEG;
}

void ConcreteBlock::setRotation(float rot)
{
	if (phys_body_ == nullptr)
		return;
	phys_body_->SetTransform(fse::FMath::sfVec2fTob2Vec2(position_), rot * FSE_DEGTORAD);
	sprite_.setRotation(rot);
	light_shape_->setRotation(rot);
}

bool ConcreteBlock::getLightOverShape() const
{
	return renderLightOver;
}

void ConcreteBlock::setLightOverShape(bool over)
{
	renderLightOver = over;
	light_shape_->setRenderLightOver(over);
}

bool ConcreteBlock::receiveShadow() const
{
	return light_shape_->receiveShadow();
}

void ConcreteBlock::setReceiveShadow(bool receive)
{
	light_shape_->setReceiveShadow(receive);
}


#include <rttr/registration>
RTTR_REGISTRATION
{
	using namespace rttr;

	registration::class_<ConcreteBlock>("ConcreteBlock")
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
	.property("size_", &ConcreteBlock::getSize, &ConcreteBlock::setSize)
	(
		metadata("CTOR_ARG", "size")
	)
	.property("rotation_", &ConcreteBlock::getRotation, &ConcreteBlock::setRotation)
	.property("light_over_shape_", &ConcreteBlock::getLightOverShape, &ConcreteBlock::setLightOverShape)
	.property("receive_shadow_", &ConcreteBlock::receiveShadow, &ConcreteBlock::setReceiveShadow)
	;
}