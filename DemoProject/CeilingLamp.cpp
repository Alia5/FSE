#include "CeilingLamp.h"
#include <FSE/Application.h>
#include <FSE/Lights/FSELightWorld.h>
#include <FSE/ImageOutlineFinder.h>
#include <FSE/FMath.h>

CeilingLamp::CeilingLamp() : CeilingLamp({0.f,0.f})
{
}

CeilingLamp::CeilingLamp(const sf::Vector2f& spawnPos) : fse::FSEObject(spawnPos)
{
	setZOrder(33);
}

CeilingLamp::~CeilingLamp()
{
	if (scene_ != nullptr)
	{
		scene_->getPhysWorld()->DestroyBody(phys_body_);
		scene_->getPhysWorld()->DestroyBody(anchor_body_);
	}
	if (!light_shapes_.empty())
	{
		for (auto& lshape : light_shapes_)
			getScene()->getLightWorld()->getLightSystem()->removeShape(lshape);
		light_shapes_.clear();
	}
}

void CeilingLamp::update(float deltaTime)
{
	auto transform = phys_body_->GetTransform();
	position_ = fse::FMath::b2Vec2ToSfVec2f(anchor_body_->GetTransform().p);
	sprite_.setRotation(transform.q.GetAngle() * FSE_RADTODEG);
	sprite_.setPosition(fse::FMath::b2Vec2ToSfVec2f(transform.p)  * FSE_PIXELS_PER_METER);
	for (auto light_shape : light_shapes_)
	{
		light_shape->setPosition(fse::FMath::b2Vec2ToSfVec2f(phys_body_->GetTransform().p) * FSE_PIXELS_PER_METER);
		light_shape->setRotation(phys_body_->GetTransform().q.GetAngle() * FSE_RADTODEG);
	}
	spot_light_.setPosition((sprite_.getPosition() + sf::Vector2f(0.f, 25.f)) * FSE_METERS_PER_PIXEL);
	spot_light_.setRotation(sprite_.getRotation());
}

void CeilingLamp::draw(sf::RenderTarget& target)
{
	auto debugRope = sf::RectangleShape(sf::Vector2f(2.5f, 
			fse::FMath::sqrtVec(
				dynamic_cast<b2RopeJoint*>(phys_body_->GetJointList()[0].joint)->GetAnchorA()
				- dynamic_cast<b2RopeJoint*>(phys_body_->GetJointList()[0].joint)->GetAnchorB()
			) * FSE_PIXELS_PER_METER));
	debugRope.setOrigin(1.25f, 0.f);
	debugRope.setFillColor(sf::Color(196, 183, 121));
	debugRope.setPosition(position_ * FSE_PIXELS_PER_METER);
	

	debugRope.setRotation(fse::FMath::angleDegree(
		dynamic_cast<b2RopeJoint*>(phys_body_->GetJointList()[0].joint)->GetAnchorA(), 
		dynamic_cast<b2RopeJoint*>(phys_body_->GetJointList()[0].joint)->GetAnchorB()) - 90);
	target.draw(debugRope);

	sprite_.render(target);
}

void CeilingLamp::drawNormals(sf::RenderTarget& target)
{
	auto debugRope = sf::RectangleShape(sf::Vector2f(2.5f,
		fse::FMath::sqrtVec(
			dynamic_cast<b2RopeJoint*>(phys_body_->GetJointList()[0].joint)->GetAnchorA()
			- dynamic_cast<b2RopeJoint*>(phys_body_->GetJointList()[0].joint)->GetAnchorB()
		) * FSE_PIXELS_PER_METER));
	debugRope.setOrigin(1.25f, 0.f);
	debugRope.setFillColor(sf::Color(0, 255, 255));
	debugRope.setPosition(position_ * FSE_PIXELS_PER_METER);

	debugRope.setRotation(fse::FMath::angleDegree(
		dynamic_cast<b2RopeJoint*>(phys_body_->GetJointList()[0].joint)->GetAnchorA(),
		dynamic_cast<b2RopeJoint*>(phys_body_->GetJointList()[0].joint)->GetAnchorB()) - 90);
	target.draw(debugRope);
	sprite_.renderNormals(target);
}

void CeilingLamp::drawSpecular(sf::RenderTarget& target)
{
	auto debugRope = sf::RectangleShape(sf::Vector2f(2.5f,
		fse::FMath::sqrtVec(
			dynamic_cast<b2RopeJoint*>(phys_body_->GetJointList()[0].joint)->GetAnchorA()
			- dynamic_cast<b2RopeJoint*>(phys_body_->GetJointList()[0].joint)->GetAnchorB()
		) * FSE_PIXELS_PER_METER));
	debugRope.setOrigin(1.25f, 0.f);
	debugRope.setFillColor(sf::Color(0, 0, 0));
	debugRope.setPosition(position_ * FSE_PIXELS_PER_METER);

	debugRope.setRotation(fse::FMath::angleDegree(
		dynamic_cast<b2RopeJoint*>(phys_body_->GetJointList()[0].joint)->GetAnchorA(),
		dynamic_cast<b2RopeJoint*>(phys_body_->GetJointList()[0].joint)->GetAnchorB()) - 90);
	target.draw(debugRope);
	sprite_.renderSpecular(target);
}

void CeilingLamp::spawned()
{
	texture_ = scene_->getApplication()->getAssetLoader().getTexture("Lamp/lamp.png");
	normal_texture_ = scene_->getApplication()->getAssetLoader().getTexture("Lamp/lamp_normal.png");
	specular_texture = scene_->getApplication()->getAssetLoader().getTexture("Lamp/lamp_spec.png");

	texture_->setSmooth(true);
	normal_texture_->setSmooth(true);
	specular_texture->setSmooth(true);

	sprite_ = ltbl::Sprite(scene_->getLightWorld()->getLightSystem());
	sprite_.setTexture(*texture_);
	sprite_.setNormalsTexture(*normal_texture_);
	sprite_.setSpecularTexture(*specular_texture);

	sprite_.setOrigin(75 / 2.f, 50 / 2.f);
	sprite_.setPosition(position_ * FSE_PIXELS_PER_METER);

	b2BodyDef physBodyDef;

	physBodyDef.type = b2_staticBody;
	physBodyDef.position.Set(position_.x, position_.y);

	anchor_body_ = getScene()->getPhysWorld()->CreateBody(&physBodyDef);
	anchor_body_->SetUserData(this);
	b2PolygonShape box;
	box.SetAsBox(0.5f, 0.5f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.friction = 1.f;
	fixtureDef.restitution = 0.001f;
	anchor_body_->CreateFixture(&fixtureDef);

	shapeFromSprite();


	spot_light_ = fse::SpotLight(scene_, sprite_.getPosition());
	spot_light_.setLenght(30);
	spot_light_.setAngle(45);

}

void CeilingLamp::setPosition(const sf::Vector2f position)
{
	position_ = position;
	anchor_body_->SetTransform(fse::FMath::sfVec2fTob2Vec2(position), anchor_body_->GetAngle());
}

sf::FloatRect CeilingLamp::GetAABBs() const
{
	return sprite_.getGlobalBounds();
}

float CeilingLamp::getRotation() const
{
	return anchor_body_->GetAngle() * FSE_RADTODEG;
}

void CeilingLamp::setRotation(float rot)
{
	anchor_body_->SetTransform(fse::FMath::sfVec2fTob2Vec2(position_), rot * FSE_DEGTORAD);
}

void CeilingLamp::shapeFromSprite()
{

	ImageOutlineFinder finder;
	finder.findOutLines(texture_->copyToImage(), alpha_threshold_);
	auto polys = finder.getSimplifiedPolys(limit_, avg_count_, avg_count_after_, max_polys_);

	if (phys_body_ != nullptr)
	{
		scene_->getPhysWorld()->DestroyBody(phys_body_);
	}

	b2BodyDef testbdef;

	testbdef.type = b2_dynamicBody;
	testbdef.position.Set(position_.x, position_.y + 10.f + 0.25f + 1.f);
	testbdef.angularDamping = 0.0f;
	testbdef.linearDamping = 0.01f;
	testbdef.bullet = true;
	testbdef.awake = true;
	testbdef.userData = this;

	phys_body_ = scene_->getPhysWorld()->CreateBody(&testbdef);


	if (!light_shapes_.empty())
	{
		for (auto& lshape : light_shapes_)
			getScene()->getLightWorld()->getLightSystem()->removeShape(lshape);
		light_shapes_.clear();
	}

	int i = 0;
	for (auto& poly : polys)
	{
		b2FixtureDef fdef;
		fdef.density = 20.f;
		fdef.friction = 0.5f;
		fdef.restitution = 0.001f;
		std::vector<b2Vec2> verts;


		for (auto & point : poly)
		{
			verts.emplace_back(point.x * FSE_METERS_PER_PIXEL, point.y * FSE_METERS_PER_PIXEL);
		}
		b2PolygonShape poly_shape;
		poly_shape.Set(verts.data(), verts.size());
		fdef.shape = &poly_shape;
		auto fixture  = phys_body_->CreateFixture(&fdef);

		sf::ConvexShape shape;
		shape.setPointCount(poly.size());
		int j = 0;
		for (auto & point : poly)
		{
			shape.setPoint(j, point);
			j++;
		}
		ltbl::LightShape* light_shape = getScene()->getLightWorld()->getLightSystem()->createLightShape(shape);
		light_shape->setPosition(fse::FMath::b2Vec2ToSfVec2f(phys_body_->GetTransform().p) * FSE_PIXELS_PER_METER);
		light_shape->setRenderLightOver(true);
		light_shape->setReceiveShadow(false);
		light_shapes_.push_back(light_shape);
		i++;
	}

	b2RopeJointDef ropeJointDef;
	ropeJointDef.bodyA = anchor_body_;
	ropeJointDef.bodyB = phys_body_;
	ropeJointDef.localAnchorA.Set(0.f, 0.f);
	ropeJointDef.localAnchorB.Set(0.f, -1.f);
	ropeJointDef.userData = this;
	ropeJointDef.maxLength = 10.f;
	ropeJointDef.collideConnected = true;


	getScene()->getPhysWorld()->CreateJoint(&ropeJointDef);

}


void CeilingLamp::setAlphaThreshold(int threshold)
{
	this->alpha_threshold_ = threshold;
	shapeFromSprite();
}

int CeilingLamp::getAlphaThreshold() const
{
	return alpha_threshold_;
}


void CeilingLamp::setLimit(float limit)
{
	limit_ = limit;
	shapeFromSprite();

}

void CeilingLamp::setAverageCount(int avgCount)
{
	avg_count_ = avgCount;
	shapeFromSprite();

}

float CeilingLamp::getLimit() const
{
	return limit_;
}

int CeilingLamp::getAverageCount() const
{
	return avg_count_;
}

#include <rttr/registration>
RTTR_REGISTRATION
{
	using namespace rttr;

	registration::class_<CeilingLamp>("CeilingLamp")
	/*.constructor<>([](fse::Scene* scene)
	{
		scene->createFSEObject<CeilingLamp>();
		return nullptr;
	})
	(
		parameter_names("scene")
		)
		.constructor([](fse::Scene* scene, const sf::Vector2f& spawnPos)
	{
		auto box = std::make_unique<CeilingLamp>(scene, spawnPos);
		scene->spawnFSEObject(std::move(box));
		return nullptr;
	})
	(
	parameter_names("scene", "spawn position")
	)*/
	.property("rotation_", &CeilingLamp::getRotation, &CeilingLamp::setRotation)
	.property("limit_", &CeilingLamp::getLimit, &CeilingLamp::setLimit)
	.property("avg_count_", &CeilingLamp::getAverageCount, &CeilingLamp::setAverageCount)
	.property("alpha_threshold_", &CeilingLamp::getAlphaThreshold, &CeilingLamp::setAlphaThreshold)
	.property("max_polys_", &CeilingLamp::max_polys_)
	.property("avg_count_after_", &CeilingLamp::avg_count_after_)
	;
}