#include "LightSystem.hpp"

#include <iostream>

namespace ltbl
{

LightSystem::LightSystem()
	: mLightShapeQuadtree(sf::FloatRect())
	, mLightPointEmissionQuadtree(sf::FloatRect())
	, mDirectionEmissionRange(1000.0f)
	, mDirectionEmissionRadiusMultiplier(1.1f)
	, mAmbientColor(sf::Color(16, 16, 16))
{
}

void LightSystem::create(const sf::FloatRect& rootRegion, const sf::Vector2u& imageSize)
{
    mLightShapeQuadtree.create(rootRegion, 6, 6);
    mLightPointEmissionQuadtree.create(rootRegion, 6, 6);

	// Load Texture
	mPenumbraTexture.loadFromMemory(priv::penumbraTexture, (sizeof(priv::penumbraTexture) / sizeof(*priv::penumbraTexture)));
	mPenumbraTexture.setSmooth(true);

	// Load Shaders
	mUnshadowShader.loadFromMemory(priv::unshadowFragment, sf::Shader::Fragment);
	mLightOverShapeShader.loadFromMemory(priv::lightOverShapeFragment, sf::Shader::Fragment);

	update(imageSize);
}

void LightSystem::render(sf::RenderTarget& target)
{
	sf::View view = target.getView();

	if (target.getSize() != mLightTempTexture.getSize())
	{
		update(target.getSize());
	}

	mLightShapeQuadtree.update();
	mLightPointEmissionQuadtree.update();

    mCompositionTexture.clear(mAmbientColor);
    mCompositionTexture.setView(mCompositionTexture.getDefaultView());

	mLightTempTexture.setView(view);

	sf::FloatRect viewBounds = sf::FloatRect(view.getCenter() - view.getSize() * 0.5f, view.getSize());

    // --- Point lights

    std::vector<priv::QuadtreeOccupant*> lightShapes;

    sf::Sprite lightTempSprite(mLightTempTexture.getTexture());

	// Query lights
	std::vector<priv::QuadtreeOccupant*> viewPointEmissionLights;
	mLightPointEmissionQuadtree.query(viewBounds, viewPointEmissionLights);

    for (const auto& occupant : viewPointEmissionLights) 
	{
		LightPointEmission* light = static_cast<LightPointEmission*>(occupant);
		if (light != nullptr && light->isTurnedOn())
		{
			// Query shapes
			lightShapes.clear();
			mLightShapeQuadtree.query(light->getAABB(), lightShapes);

			// Render on Emission Texture : used by lightOverShapeShader
			mEmissionTempTexture.clear();
			mEmissionTempTexture.setView(view);
			mEmissionTempTexture.draw(*light);
			mEmissionTempTexture.display();

			// Render light
			light->render(view, mLightTempTexture, mAntumbraTempTexture, mUnshadowShader, mLightOverShapeShader, lightShapes);
			mCompositionTexture.draw(lightTempSprite, sf::BlendAdd);
		}
    }

    //----- Direction lights

	float shadowExtension = priv::vectorMagnitude(view.getSize() * -0.5f) * mDirectionEmissionRadiusMultiplier;
	sf::Vector2f extendedBounds = sf::Vector2f(1.f, 1.f) * std::max(viewBounds.width, viewBounds.height) * mDirectionEmissionRadiusMultiplier;
	sf::FloatRect extendedViewBounds = priv::rectFromBounds(-extendedBounds, extendedBounds + sf::Vector2f(mDirectionEmissionRange, 0.0f));

	std::vector<priv::QuadtreeOccupant*> viewLightShapes;

    for (const auto& light : mDirectionEmissionLights) 
	{
		// Create light shape
        sf::ConvexShape directionShape = priv::shapeFromRect(extendedViewBounds);
        directionShape.setPosition(view.getCenter());
        directionShape.setRotation(light->getCastAngle());

		// Query shapes
		viewLightShapes.clear();
        mLightShapeQuadtree.query(directionShape, viewLightShapes);

		// Render light
        light->render(view, mLightTempTexture, mAntumbraTempTexture, mUnshadowShader, viewLightShapes, shadowExtension);
        mCompositionTexture.draw(sf::Sprite(mLightTempTexture.getTexture()), sf::BlendAdd);
    }

    mCompositionTexture.display();

	target.setView(target.getDefaultView());
	target.draw(sf::Sprite(mCompositionTexture.getTexture()), sf::BlendMultiply);
	target.setView(view);
}

LightShape* LightSystem::createLightShape()
{
	LightShape* shape = new LightShape();
	mLightShapeQuadtree.addOccupant(shape);
	mLightShapes.insert(shape);
	return shape;
}

LightShape* LightSystem::createLightShape(const sf::RectangleShape& shape)
{
	LightShape* lightShape = createLightShape();
	lightShape->setPointCount(4u);
	lightShape->setPoint(0u, { 0.f, 0.f });
	lightShape->setPoint(1u, { shape.getSize().x, 0.f });
	lightShape->setPoint(2u, shape.getSize());
	lightShape->setPoint(3u, { 0.f, shape.getSize().y });
	lightShape->setPosition(shape.getPosition());
	lightShape->setOrigin(shape.getOrigin());
	lightShape->setRotation(shape.getRotation());
	lightShape->setScale(shape.getScale());
	return lightShape;
}

LightShape* LightSystem::createLightShape(const sf::ConvexShape& shape)
{
	LightShape* lightShape = createLightShape();
	unsigned int pointCount = shape.getPointCount();
	lightShape->setPointCount(pointCount);
	for (unsigned int i = 0; i < pointCount; i++)
	{
		lightShape->setPoint(i, shape.getPoint(i));
	}
	lightShape->setPosition(shape.getPosition());
	lightShape->setOrigin(shape.getOrigin());
	lightShape->setRotation(shape.getRotation());
	lightShape->setScale(shape.getScale());
	return lightShape;
}

LightShape* LightSystem::createLightShape(const sf::CircleShape& shape)
{
	LightShape* lightShape = createLightShape();
	unsigned int pointCount = shape.getPointCount();
	lightShape->setPointCount(pointCount);
	for (unsigned int i = 0; i < pointCount; i++)
	{
		lightShape->setPoint(i, shape.getPoint(i));
	}
	lightShape->setPosition(shape.getPosition());
	lightShape->setOrigin(shape.getOrigin());
	lightShape->setRotation(shape.getRotation());
	lightShape->setScale(shape.getScale());
	return lightShape;
}

LightShape* LightSystem::createLightShape(const sf::FloatRect& rect)
{
	LightShape* lightShape = createLightShape();
	lightShape->setPointCount(4u);
	lightShape->setPoint(0u, { 0.f, 0.f });
	lightShape->setPoint(1u, { rect.width, 0.f });
	lightShape->setPoint(2u, { rect.width, rect.height });
	lightShape->setPoint(3u, { 0.f, rect.height });
	lightShape->setPosition(rect.left, rect.top);
	return lightShape;
}

LightShape* LightSystem::createLightShape(const sf::Sprite& sprite)
{
	LightShape* lightShape = createLightShape();
	lightShape->setPointCount(4u);
	lightShape->setPoint(0u, { 0.f, 0.f });
	lightShape->setPoint(1u, { sprite.getTextureRect().width * 1.f, 0.f });
	lightShape->setPoint(2u, { sprite.getTextureRect().width * 1.f, sprite.getTextureRect().height * 1.f });
	lightShape->setPoint(3u, { 0.f, sprite.getTextureRect().height * 1.f });
	lightShape->setPosition(sprite.getPosition());
	lightShape->setOrigin(sprite.getOrigin());
	lightShape->setRotation(sprite.getRotation());
	lightShape->setScale(sprite.getScale());
	return lightShape;
}

void LightSystem::removeShape(LightShape* shape)
{
	auto itr = mLightShapes.find(shape);
	if (itr != mLightShapes.end()) 
	{
		mLightShapeQuadtree.removeOccupant(*itr);
		mLightShapes.erase(itr);
		delete shape;
	}
}

LightPointEmission* LightSystem::createLightPointEmission()
{
	LightPointEmission* light = new LightPointEmission();
	mLightPointEmissionQuadtree.addOccupant(light);
	mPointEmissionLights.insert(light);
	return light;
}

void LightSystem::removeLight(LightPointEmission* light)
{
	auto itr = mPointEmissionLights.find(light);
	if (itr != mPointEmissionLights.end())
	{
		mLightPointEmissionQuadtree.removeOccupant(*itr);
		mPointEmissionLights.erase(itr);
		delete light;
	}
}

LightDirectionEmission* LightSystem::createLightDirectionEmission()
{
	LightDirectionEmission* light = new LightDirectionEmission();
	mDirectionEmissionLights.insert(light);
	return light;
}

void LightSystem::removeLight(LightDirectionEmission* light) 
{
    auto itr = mDirectionEmissionLights.find(light);
	if (itr != mDirectionEmissionLights.end())
	{
		mDirectionEmissionLights.erase(itr);
		delete light;
	}
}

void LightSystem::setDirectionEmissionRange(float range)
{
	mDirectionEmissionRange = range;
}

float LightSystem::getDirectionEmissionRange() const
{
	return mDirectionEmissionRange;
}

void LightSystem::setDirectionEmissionRadiusMultiplier(float multiplier)
{
	mDirectionEmissionRadiusMultiplier = multiplier;
}

float LightSystem::getDirectionEmissionRadiusMultiplier() const
{
	return mDirectionEmissionRadiusMultiplier;
}

void LightSystem::setAmbientColor(const sf::Color& color)
{
	mAmbientColor = color;
}

const sf::Color& LightSystem::getAmbientColor() const
{
	return mAmbientColor;
}

void LightSystem::update(sf::Vector2u const& size)
{
	mUnshadowShader.setUniform("penumbraTexture", mPenumbraTexture);

	if (size.x != 0 && size.y != 0)
	{
		mLightTempTexture.create(size.x, size.y);
		mEmissionTempTexture.create(size.x, size.y);
		mAntumbraTempTexture.create(size.x, size.y);
		mCompositionTexture.create(size.x, size.y);

		mLightOverShapeShader.setUniform("emissionTexture", mEmissionTempTexture.getTexture());
		mLightOverShapeShader.setUniform("targetSizeInv", sf::Vector2f(1.0f / size.x, 1.0f / size.y));
	}
}

sf::Texture& LightSystem::getPenumbraTexture()
{
	return mPenumbraTexture;
}

sf::Shader& LightSystem::getUnshadowShader()
{
	return mUnshadowShader;
}

sf::Shader& LightSystem::getLightOverShapeShader()
{
	return mLightOverShapeShader;
}

} // namespace ltbl