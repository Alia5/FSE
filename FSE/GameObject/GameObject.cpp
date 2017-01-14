#include "GameObject.h"
#include "../Game.h"

namespace fse
{

	GameObject::GameObject(Scene *scene) : m_position(sf::Vector2f(0,0))
	{
		m_Scene = scene;
		m_input = scene->getGame()->getInput();
	}

	GameObject::GameObject(Scene* scene, const sf::Vector2f spawnPos) : GameObject(scene)
	{

	}


	GameObject::~GameObject()
	{
		std::wcout << "Destroying Game Object Base; id: " << m_ID << std::endl;
	}

	void GameObject::setPosition(const sf::Vector2f position)
	{
		m_position = position;
	}

	sf::Vector2f GameObject::getPosition()
	{
		return m_position;
	}

	void GameObject::BeginContact(GameObject* otherObject, b2Contact* contact)
	{
	}

	void GameObject::EndContact(GameObject* otherObject, b2Contact* contact)
	{
	}

	void GameObject::PreSolve(GameObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold)
	{
	}

	void GameObject::PostSolve(GameObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
	{
	}


	int GameObject::getZOrder() const
	{
		return m_ZOrder;
	}

	bool GameObject::isPendingKill() const
	{
		if (m_pendingTimedKill)
			return true;
		return m_isPendingKill;
	}

	void GameObject::setTimedKill()
	{
		m_pendingTimedKill = true;
	}


	Scene* GameObject::getScene()
	{
		return m_Scene;
	}

	void GameObject::spawn(int id)
	{
		m_ID = id;
		spawned();
		spawnedSignal(this);
		spawnedSignal.disconnectAll();
	}

	bool GameObject::destroy()
	{
		if (!m_isPendingKill)
		{
			m_isPendingKill = true;

			m_Scene->destroyGameObject(this);

			return true;
		}
		return false;
	}

	int GameObject::getID()
	{
		return m_ID;
	}

	void GameObject::setZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
		m_Scene->notifyZOrderChanged();
	}

	std::vector<std::unique_ptr<GameObject>>* GameObject::getSceneGameObjects() const
	{
		return m_Scene->getGameObjects();
	}

}
