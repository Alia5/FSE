#include "KillVolume.h"
#include "../FMath.h"
#include <rttr/registration>

namespace fse
{
	KillVolume::KillVolume() : KillVolume({ 0.f, 0.f })
	{
	}


	KillVolume::KillVolume(const sf::Vector2f& spawnPos) : KillVolume(spawnPos, { 1.f, 1.f })
	{
	}


	KillVolume::KillVolume(const sf::Vector2f& spawnPos, const sf::Vector2f& size)
		: FSEObject(spawnPos), size_(size), phys_body_(nullptr), sensor_ficture_(nullptr)
	{
		if (size_.x == 0.f)
			size_.x = 1.f;

		if (size_.y == 0.f)
			size_.y = 1.f;

		position_ = spawnPos;
	}


	KillVolume::~KillVolume()
	{
	}


	void KillVolume::setPosition(const sf::Vector2f position)
	{
		position_ = position;
		if (phys_body_ != nullptr)
			phys_body_->SetTransform(fse::FMath::sfVec2fTob2Vec2(position), phys_body_->GetAngle());
	}


	void KillVolume::update(float deltaTime)
	{
	}


	void KillVolume::draw(sf::RenderTarget& target)
	{
	}


	void KillVolume::spawned()
	{
		b2BodyDef physBodyDef;

		physBodyDef.type = b2_staticBody;
		physBodyDef.position.Set(position_.x, position_.y);

		phys_body_ = getScene()->getPhysWorld()->CreateBody(&physBodyDef);
		phys_body_->SetUserData(this);

		b2PolygonShape box;
		box.SetAsBox(size_.x / 2.f, size_.y / 2.f);

		b2FixtureDef sensorDef;
		sensorDef.shape = &box;
		sensorDef.isSensor = true;

		sensor_ficture_ = phys_body_->CreateFixture(&sensorDef);
	}

	void KillVolume::onDespawn()
	{
		scene_->getPhysWorld()->DestroyBody(phys_body_);
	}


	void KillVolume::BeginContact(FSEObject* otherObject, b2Contact* contact)
	{
		otherObject->destroy();
	}


	sf::FloatRect KillVolume::GetAABBs() const
	{
		if (scene_ == nullptr)
			return sf::FloatRect();
		return sf::FloatRect(
			(phys_body_->GetPosition().x - size_.x / 2.f) * scene_->getPixelsPerMeter(),
			(phys_body_->GetPosition().y - size_.y / 2.f) * scene_->getPixelsPerMeter(),
			size_.x * scene_->getPixelsPerMeter(),
			size_.y * scene_->getPixelsPerMeter());
	}

	void KillVolume::setSize(const sf::Vector2f& size)
	{
		size_ = size;
		if (sensor_ficture_ != nullptr)
		{
			b2PolygonShape* shape = static_cast<b2PolygonShape*>(sensor_ficture_->GetShape());
			shape->SetAsBox(size_.x / 2.f, size_.y / 2.f);
		}
	}

	const sf::Vector2f& KillVolume::getSize() const
	{
		return size_;
	}


	FSE_V8_REGISTER(KillVolume)
	{

		v8::HandleScope handle_scope(isolate);
		v8pp::class_<KillVolume, v8pp::shared_ptr_traits>KillVolume_class(isolate);
		KillVolume_class.inherit<FSEObject>();
		fse::addV8DownCastHelper<fse::FSEObject, fse::KillVolume>();
		KillVolume_class.auto_wrap_objects(true);
		KillVolume_class.ctor<void>(
			[](v8::FunctionCallbackInfo<v8::Value> const& args)
				{
					// TODO: overloads
					return std::make_shared<KillVolume>();
				});
		KillVolume_class.function("getSize", static_cast<const sf::Vector2f&(KillVolume::*)() const>(&KillVolume::getSize));
		KillVolume_class.function("setSize", static_cast<void(KillVolume::*)(const sf::Vector2f& size)>(&KillVolume::setSize));
		module.class_("KillVolume", KillVolume_class);
	}

}

RTTR_REGISTRATION
{
	using namespace rttr;

	registration::class_<fse::KillVolume>("fse::KillVolume")
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
	;
}