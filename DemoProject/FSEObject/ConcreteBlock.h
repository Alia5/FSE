#pragma once
#include <FSE/FSEObject/FSEObject.h>
#include <LTBL2/src/LightShape.hpp>
#include <FSE/AssetLoader.h>
#include <LTBL2/src/Sprite.hpp>

class ConcreteBlock : public fse::FSEObject
{
public:
	ConcreteBlock();
	explicit ConcreteBlock(const sf::Vector2f& spawnPos);
	explicit ConcreteBlock(const sf::Vector2f& spawnPos, const sf::Vector2f& size);
	~ConcreteBlock() override;

	void update(float deltaTime) override;
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

	bool getLightOverShape() const;
	void setLightOverShape(bool over);

	bool receiveShadow() const;
	void setReceiveShadow(bool receive);

private:

	sf::Vector2f size_;
	ltbl::LightShape* light_shape_;
	b2Body *phys_body_;
	b2Fixture* fixture_;

	fse::Texture texture_;
	fse::Texture normal_texture_;
	fse::Texture specular_texture;

	ltbl::Sprite sprite_;

	bool renderLightOver = true;


	RTTR_ENABLE(fse::FSEObject)
	RTTR_REGISTRATION_FRIEND

};
