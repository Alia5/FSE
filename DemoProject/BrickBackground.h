#pragma once
#include <FSE/FSEObject/FSEObject.h>
#include <LTBL2/src/LightShape.hpp>
#include <FSE/AssetLoader.h>
#include <LTBL2/src/Sprite.hpp>

class BrickBackground : public fse::FSEObject
{
public:

	explicit BrickBackground(fse::Scene* scene);
	BrickBackground(fse::Scene* scene, const sf::Vector2f& spawnPos);
	BrickBackground(fse::Scene* scene, const sf::Vector2f& spawnPos, const sf::Vector2f& size);
	~BrickBackground() = default;

	void draw(sf::RenderTarget& target) override;
	void drawNormals(sf::RenderTarget& target) override;
	void drawSpecular(sf::RenderTarget& target) override;
	void spawned() override;

	void setPosition(const sf::Vector2f position) override;
	sf::FloatRect GetAABBs() const override;

	const sf::Vector2f& getSize();
	void setSize(const sf::Vector2f& size);

	float getRotation() const;
	void setRotation(float rot);

private:
	fse::Texture texture_;
	fse::Texture normal_texture_;
	fse::Texture specular_texture;

	sf::Vector2f size_;
	ltbl::Sprite sprite_;



	RTTR_ENABLE(fse::FSEObject)
	RTTR_REGISTRATION_FRIEND
};

