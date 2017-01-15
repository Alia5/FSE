#include "FSEObject.h"
#include "../Application.h"

namespace fse
{

	FSEObject::FSEObject(Scene *scene) : m_position(sf::Vector2f(0,0))
	{
		m_Scene = scene;
		m_input = scene->getApplication()->getInput();
	}

	FSEObject::FSEObject(Scene* scene, const sf::Vector2f spawnPos) : FSEObject(scene)
	{

	}


	FSEObject::~FSEObject()
	{
		std::wcout << "Destroying Application Object Base; id: " << m_ID << std::endl;
	}

	void FSEObject::setPosition(const sf::Vector2f position)
	{
		m_position = position;
	}

	sf::Vector2f FSEObject::getPosition()
	{
		return m_position;
	}

	void FSEObject::BeginContact(FSEObject* otherObject, b2Contact* contact)
	{
	}

	void FSEObject::EndContact(FSEObject* otherObject, b2Contact* contact)
	{
	}

	void FSEObject::PreSolve(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold)
	{
	}

	void FSEObject::PostSolve(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
	{
	}


	int FSEObject::getZOrder() const
	{
		return m_ZOrder;
	}

	bool FSEObject::isPendingKill() const
	{
		if (m_pendingTimedKill)
			return true;
		return m_isPendingKill;
	}

	void FSEObject::setTimedKill()
	{
		m_pendingTimedKill = true;
	}


	Scene* FSEObject::getScene()
	{
		return m_Scene;
	}

	void FSEObject::spawn(int id)
	{
		m_ID = id;
		spawned();
		spawnedSignal(this);
		spawnedSignal.disconnectAll();
	}

	bool FSEObject::destroy()
	{
		if (!m_isPendingKill)
		{
			m_isPendingKill = true;

			m_Scene->destroyFSEObject(this);

			return true;
		}
		return false;
	}

	int FSEObject::getID()
	{
		return m_ID;
	}

	void FSEObject::setZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
		m_Scene->notifyZOrderChanged();
	}

	std::vector<std::unique_ptr<FSEObject>>* FSEObject::getSceneFSEObjects() const
	{
		return m_Scene->getFSEObjects();
	}

}
