#include "Scene.h"
#include "Application.h"
#include "FSEObject/FSEObject.h"
#include "Lights/FSELightWorld.h"

namespace fse
{

	Scene::Scene(Application* application) : m_renderTarget(nullptr), m_Application(application), /*physGravity(0.f,10.f),*/ physWorld(physGravity)
	{
		physWorld.SetContactListener(&phys_contact_listener);
		WinResizeSignalConnection = m_Application->onWindowResized.connect(this, &Scene::notifyResize);

		createFSEObject<FSELightWorld>([this](FSEObject* lightWorld)
		{
			m_light_world = dynamic_cast<FSELightWorld*>(lightWorld);
		});
		processPendingSpawns();
	}

	Scene::~Scene()
	{
		m_Application->onWindowResized.disconnect(WinResizeSignalConnection);

		m_FSEObjects.clear();
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


		m_light_world->init(renderTarget);

	}


	void Scene::update(float deltaTime)
	{
		if (paused)
			deltaTime = 0.0f;

		physAccumulator += deltaTime;

		while (physAccumulator >= physTimestep)
		{
			physWorld.Step(physTimestep, physVelocyIters, physPosIters);
			physAccumulator -= physTimestep;
		}

		for (auto it = m_FSEObjects.rbegin(); it != m_FSEObjects.rend(); ++it)
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
			std::sort(m_FSEObjects.begin(), m_FSEObjects.end(),
				[](const std::unique_ptr<FSEObject>& a, const std::unique_ptr<FSEObject>& b) {

				return a->getZOrder() < b->getZOrder();

			});


			m_ZOrderChanged = false;
		}
		renderer->render(m_FSEObjects);

		if (phys_draw_debug)
		{
			physWorld.DrawDebugData();
		}
	}

	std::vector<std::unique_ptr<FSEObject> >* Scene::getFSEObjects()
	{
		return &m_FSEObjects;
	}

	void Scene::notifyZOrderChanged()
	{
		m_ZOrderChanged = true;
	}



	void Scene::spawnFSEObject(std::unique_ptr<FSEObject> object)
	{
		m_pendingObjectSpawns.push_back(std::move(object));
	}

	void Scene::destroyFSEObject(FSEObject* object)
	{
		m_pendingObjectRemovals.push_back(object);
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
		m_renderTarget->setView(view);
	}

	Application* Scene::getApplication() const
	{
		return m_Application;
	}

	FSELightWorld* Scene::getLightWorld() const
	{
		return m_light_world;
	}


	b2World *Scene::getPhysWorld()
	{
		return &physWorld;
	}


	void Scene::addFSEObject(std::unique_ptr<FSEObject> object)
	{
		object->spawn(spawnCount++);
		m_FSEObjects.push_back(std::move(object));
		m_ZOrderChanged = true;
	}

	void Scene::removeFSEObject(std::unique_ptr<FSEObject> const& object)
	{
		auto it = std::find(m_FSEObjects.begin(), m_FSEObjects.end(), object);
		if (it != m_FSEObjects.end())
		{
			m_FSEObjects.erase(it);
		}
	}

	void Scene::removeFSEObject(FSEObject * object)
	{
		auto it = std::find_if(m_FSEObjects.begin(), m_FSEObjects.end(), [&](const std::unique_ptr<FSEObject> & obj) {
			return obj.get() == object;
		});
		if (it != m_FSEObjects.end())
		{
			m_FSEObjects.erase(it);
		}
	}

	void Scene::processPendingRemovals()
	{
		if (m_pendingObjectRemovals.size() > 0)
		{
			for (auto & object : m_pendingObjectRemovals)
			{
				removeFSEObject(object);
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
				addFSEObject(std::move(object));
			}
			m_pendingObjectSpawns.clear();
		}
	}

}