#include "FSELightWorld.h"
#include "../Application.h"

namespace fse
{
	FSELightWorld::FSELightWorld(Scene* scene) : FSELightWorld(scene, sf::Vector2f(0, 0))
	{
		m_light_system = std::make_unique<ltbl::LightSystem>();
		onResizeConnection = getScene()->getApplication()->onWindowResized.connect([this]()
		{
			//m_light_system->update(getScene()->getApplication()->getWindow()->getSize());
		});
	}

	FSELightWorld::FSELightWorld(Scene* scene, const sf::Vector2f& spawnPos) : FSEObject(scene, spawnPos)
	{
		setZOrder(255);
	}

	FSELightWorld::~FSELightWorld()
	{
		getScene()->getApplication()->onWindowResized.disconnect(onResizeConnection);
		m_light_system->removeLight(m_Sun);
	}

	void FSELightWorld::update(float deltaTime)
	{
	}

	void FSELightWorld::draw(sf::RenderTarget& target)
	{
		if (m_lighting)
			m_light_system->render(target);
	}

	void FSELightWorld::spawned()
	{
	}

	void FSELightWorld::init(sf::RenderTarget* target)
	{
		m_light_system->create({ -1000.f, -1000.f ,1000.f ,1000.f }, target->getSize());
		m_Sun = m_light_system->createLightDirectionEmission();
		m_Sun->setColor(sf::Color::Black);
	}

	bool FSELightWorld::getLighting() const
	{
		return m_lighting;
	}

	void FSELightWorld::setLighting(bool lighting)
	{
		m_lighting = lighting;
	}

	ltbl::LightDirectionEmission* FSELightWorld::getSun() const
	{
		return m_Sun;
	}

	ltbl::LightSystem* FSELightWorld::getLightSystem()
	{
		return m_light_system.get();
	}
}
