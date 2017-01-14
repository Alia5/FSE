#include "Scene.h"
#include "Game.h"
#include "GameObject/GameObject.h"

namespace fse
{

	Scene::Scene(Game* game) : m_renderTarget(nullptr), m_game(game), /*physGravity(0.f,10.f),*/ physWorld(physGravity)
	{
		physWorld.SetContactListener(&phys_contact_listener);
		gameSignalConnection = m_game->onWindowResized.connect(this, &Scene::notifyResize);
	}

	Scene::~Scene()
	{
		m_game->onWindowResized.disconnect(gameSignalConnection);

		m_gameObjects.clear();
		m_pendingObjectRemovals.clear();
		m_pendingObjectSpawns.clear();

		std::wcout << "Deleting scene\n";
	}

	void Scene::setRenderTarget(sf::RenderTarget * renderTarget)
	{
		m_renderTarget = renderTarget;
		renderer = std::make_unique<Renderer>(m_renderTarget);

		phys_debug_draw = PhysDebugDraw(*m_renderTarget);
		physWorld.SetDebugDraw(&phys_debug_draw);
	}


	void Scene::update(float deltaTime)
	{
		if (paused)
			deltaTime = 0.0f;

		physAccumulator += deltaTime;

		//int numSteps = 0;

		while (physAccumulator >= physTimestep)
		{
			physWorld.Step(physTimestep, physVelocyIters, physPosIters);
			physAccumulator -= physTimestep;

			//physWorld.ClearForces();

			//numSteps++;
			//testCounter++;
		}

		//if (testTimer.getElapsedTime().asSeconds() >= 1)
		//{
		//	std::wcout << "steps: " << testCounter << "\n";
		//	testTimer.restart();
		//	testCounter = 0;
		//}
		//std::wcout << "physStepTimeLeft: " << physStepTimeLeft << "\n" << "took " << numSteps << " steps\n";

		//for (auto &object : m_gameObjects)
		//{
		//	object->update(deltaTime);
		//}

		for (auto it = m_gameObjects.rbegin(); it != m_gameObjects.rend(); ++it)
		{
			it->get()->update(deltaTime);
		}

		processPendingRemovals();
		processPendingSpawns();
	}

	void Scene::draw()
	{
		if (m_ZOrderChanged)
		{
			std::sort(m_gameObjects.begin(), m_gameObjects.end(),
				[](const std::unique_ptr<GameObject>& a, const std::unique_ptr<GameObject>& b) {

				return a->getZOrder() < b->getZOrder();

			});


			m_ZOrderChanged = false;
		}
		renderer->render(m_gameObjects);

		if (phys_draw_debug)
		{
			physWorld.DrawDebugData();
		}
	}

	std::vector<std::unique_ptr<GameObject> >* Scene::getGameObjects()
	{
		return &m_gameObjects;
	}

	void Scene::notifyZOrderChanged()
	{
		m_ZOrderChanged = true;
	}



	void Scene::spawnGameObject(std::unique_ptr<GameObject> gameObject)
	{
		m_pendingObjectSpawns.push_back(std::move(gameObject));
	}

	void Scene::destroyGameObject(GameObject* gameObject)
	{
		m_pendingObjectRemovals.push_back(gameObject);
	}

	bool Scene::isPaused()
	{
		return paused;
	}

	void Scene::setPaused(bool paused)
	{
		this->paused = paused;
	}

	bool Scene::getPhysDrawDebug() const
	{
		return phys_draw_debug;
	}

	void Scene::setPhysDrawDebug(bool drawDebug)
	{
		phys_draw_debug = drawDebug;
	}

	PhysDebugDraw& Scene::getPhysDebugDraw()
	{
		return phys_debug_draw;
	}

	void Scene::notifyResize()
	{
		sf::View view = m_renderTarget->getView();
		sf::IntRect viewPort = m_renderTarget->getViewport(view);
		view.setSize(sf::Vector2f(static_cast<float>(viewPort.width), static_cast<float>(viewPort.height)));
		//view.reset(sf::FloatRect(0, 0, viewPort.width, viewPort.height));
		m_renderTarget->setView(view);
	}

	Game* Scene::getGame() const
	{
		return m_game;
	}

	b2World *Scene::getPhysWorld()
	{
		return &physWorld;
	}


	void Scene::addGameObject(std::unique_ptr<GameObject> gameObject)
	{
		gameObject->spawn(spawnCount++);
		m_gameObjects.push_back(std::move(gameObject));
		m_ZOrderChanged = true;
	}

	void Scene::removeGameObject(std::unique_ptr<GameObject> const& gameObject)
	{
		auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject);
		if (it != m_gameObjects.end())
		{
			std::wcout << "Deleting object " << typeid(it->get()).name() << std::endl;
			m_gameObjects.erase(it);
		}
	}

	void Scene::removeGameObject(GameObject * gameObject)
	{
		auto it = std::find_if(m_gameObjects.begin(), m_gameObjects.end(), [&](const std::unique_ptr<GameObject> & obj) {
			return obj.get() == gameObject;
		});
		if (it != m_gameObjects.end())
		{
			std::wcout << "Deleting object " << typeid(it->get()).name() << std::endl;
			m_gameObjects.erase(it);
		}
	}

	void Scene::processPendingRemovals()
	{
		if (m_pendingObjectRemovals.size() > 0)
		{
			for (auto & object : m_pendingObjectRemovals)
			{
				removeGameObject(object);
			}
			m_pendingObjectRemovals.clear();
		}

	}

	void Scene::processPendingSpawns()
	{
		if (m_pendingObjectSpawns.size() > 0)
		{
			for (auto & object : m_pendingObjectSpawns)
			{
				addGameObject(std::move(object));
			}
			m_pendingObjectSpawns.clear();
		}
	}

}