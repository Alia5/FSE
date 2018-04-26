#pragma once
#include <FSE/FSEObject/FSEObject.h>
#include <LTBL2/src/LightShape.hpp>
#include <FSE/AssetLoader.h>
#include <LTBL2/src/Sprite.hpp>
#include <FSE/Lights/SpotLight.h>

class CeilingLamp : public fse::FSEObject
{
public:
	explicit CeilingLamp(fse::Scene* scene);
	CeilingLamp(fse::Scene* scene, const sf::Vector2f& spawnPos);
	~CeilingLamp() override;


	void update(float deltaTime) override;
	void draw(sf::RenderTarget& target) override;
	void drawNormals(sf::RenderTarget& target) override;
	void drawSpecular(sf::RenderTarget& target) override;
	void spawned() override;

	void setPosition(const sf::Vector2f position) override;
	sf::FloatRect GetAABBs() const override;

	float getRotation() const;
	void setRotation(float rot);

private:

	void shapeFromSprite();


	void setAlphaThreshold(int threshold);
	int getAlphaThreshold() const;

	void setLimit(float limit);
	void setAverageCount(int avgCount);

	float getLimit() const;
	int getAverageCount() const;

	fse::Texture texture_;
	fse::Texture normal_texture_;
	fse::Texture specular_texture;

	ltbl::Sprite sprite_;

	fse::SpotLight spot_light_;

	ltbl::LightShape* light_shape_;
	b2Body *anchor_body_;
	b2Body *phys_body_;
	std::vector<ltbl::LightShape*> light_shapes_;


	float limit_ = 0.186f;
	int avg_count_ = 1;
	int avg_count_after_ = 0;
	int alpha_threshold_ = 29;
	int max_polys_ = 5;

	RTTR_ENABLE(fse::FSEObject)
	RTTR_REGISTRATION_FRIEND

};


