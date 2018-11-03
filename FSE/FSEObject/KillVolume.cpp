#include "KillVolume.h"
#include "../FMath.h"
#include <rttr/registration>

namespace fse
{
	KillVolume::KillVolume(fse::Scene* scene) : KillVolume(scene, { 0.f, 0.f })
	{
	}


	KillVolume::KillVolume(fse::Scene* scene, const sf::Vector2f& spawnPos) : KillVolume(scene, spawnPos, { 1.f, 1.f })
	{
	}


	KillVolume::KillVolume(fse::Scene* scene, const sf::Vector2f& spawnPos, const sf::Vector2f& size) 
	: FSEObject(scene, spawnPos), size_(size)
	{
		if (size_.x == 0.f)
			size_.x = 1.f;

		if (size_.y == 0.f)
			size_.y = 1.f;

		b2BodyDef physBodyDef;

		physBodyDef.type = b2_staticBody;
		physBodyDef.position.Set(spawnPos.x, spawnPos.y);

		phys_body_ = getScene()->getPhysWorld()->CreateBody(&physBodyDef);
		phys_body_->SetUserData(this);

		b2PolygonShape box;
		box.SetAsBox(size_.x / 2.f, size_.y / 2.f);

		b2FixtureDef sensorDef;
		sensorDef.shape = &box;
		sensorDef.isSensor = true;

		sensor_ficture_ = phys_body_->CreateFixture(&sensorDef);
	}


	KillVolume::~KillVolume()
	{
		scene_->getPhysWorld()->DestroyBody(phys_body_);
	}


	void KillVolume::setPosition(const sf::Vector2f position)
	{
		position_ = position;
		phys_body_->SetTransform(fse::FMath::sfVec2fTob2Vec2(position), phys_body_->GetAngle());
	}


	void KillVolume::update(float deltaTime)
	{
	}


	void KillVolume::draw(sf::RenderTarget & target)
	{
	}


	void KillVolume::spawned()
	{
	}


	void KillVolume::BeginContact(FSEObject * otherObject, b2Contact * contact)
	{
		otherObject->destroy();
	}


	sf::FloatRect KillVolume::GetAABBs() const
	{
		return sf::FloatRect(
			(phys_body_->GetPosition().x - size_.x / 2.f) * FSE_PIXELS_PER_METER,
			(phys_body_->GetPosition().y - size_.y / 2.f) * FSE_PIXELS_PER_METER,
			size_.x * FSE_PIXELS_PER_METER,
			size_.y * FSE_PIXELS_PER_METER );
	}

	void KillVolume::setSize(const sf::Vector2f& size)
	{
		size_ = size;
		b2PolygonShape* shape = static_cast<b2PolygonShape*>(sensor_ficture_->GetShape());
		shape->SetAsBox(size_.x / 2.f, size_.y / 2.f);
	}

	const sf::Vector2f& KillVolume::getSize() const
	{
		return size_;
	}

}
