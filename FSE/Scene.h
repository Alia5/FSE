#pragma once

#include <vector>
#include <Box2D/Box2D.h>

#include "Renderer.h"

//#include "EventBus.h"

#include "Signals.h"
#include "PhysContactListener.h"
#include "PhysDebugDraw.h"
#include "NetworkHandler.h"

#define FSE_RADTODEG 57.2958f
#define FSE_PXMRATIO = 25.f
#define FSE_PIXELS_PER_METER 25.f
#define FSE_METERS_PER_PIXEL (1.f/25.f)

namespace fse
{
	class Game;
	class GameObject;
	class Scene 
	{
	public:
		Scene(Game *game);
		~Scene();

		void setRenderTarget(sf::RenderTarget* renderTarget);
		
		void update(float deltaTime);
		void draw();

		std::vector<std::unique_ptr<GameObject>>* getGameObjects();

		void notifyZOrderChanged();

		void spawnGameObject(std::unique_ptr<GameObject> gameObject);

		template<typename T>
		void createGameObject()
		{
			std::unique_ptr<T> gameObject = std::unique_ptr<T>(new T(this));
			m_pendingObjectSpawns.push_back(std::move(gameObject));
		}

		template<typename T, typename SpawnedSlot>
		void createGameObject(SpawnedSlot&& slot)
		{
			std::unique_ptr<T> gameObject = std::unique_ptr<T>(new T(this));
			gameObject->spawnedSignal.connect(slot);
			m_pendingObjectSpawns.push_back(std::move(gameObject));
		}

		template<typename T>
		void createGameObject(const sf::Vector2f spawnPos)
		{
			std::unique_ptr<T> gameObject = std::unique_ptr<T>(new T(this, spawnPos));
			m_pendingObjectSpawns.push_back(std::move(gameObject));
		}

		template<typename T, typename SpawnedSlot>
		void createGameObject(const sf::Vector2f spawnPos, SpawnedSlot&& slot)
		{
			std::unique_ptr<T> gameObject = std::unique_ptr<T>(new T(this, spawnPos));
			gameObject->spawnedSignal.connect(slot);
			m_pendingObjectSpawns.push_back(std::move(gameObject));
		}

		void destroyGameObject(GameObject* gameObject);

		bool isPaused();
		void setPaused(bool paused);

		bool getPhysDrawDebug() const;
		void setPhysDrawDebug(bool drawDebug);
		PhysDebugDraw& getPhysDebugDraw();

		void notifyResize();

		Game* getGame() const;

		b2World *getPhysWorld();


	protected:
		sf::RenderTarget* m_renderTarget;

		std::vector<std::unique_ptr<GameObject> > m_gameObjects; 

		std::list<std::unique_ptr<GameObject> > m_pendingObjectSpawns;
		std::list<GameObject*> m_pendingObjectRemovals;


		std::unique_ptr<Renderer> renderer;

		void addGameObject(std::unique_ptr<GameObject> gameObject);
		void removeGameObject(std::unique_ptr<GameObject> const & gameObject);

		void removeGameObject(GameObject* gameObject);

		void processPendingRemovals();
		void processPendingSpawns();

	private:
		bool m_ZOrderChanged = false;
		int spawnCount = 0;

		bool paused = false;

		float elapsedTime = 0;

		Game *m_game;
		Signal<>::Connection gameSignalConnection;

		NetworkHandler* network_handler_ = nullptr;
		
		const int32 physVelocyIters = 16;
		const int32 physPosIters = 6;
		b2Vec2 physGravity = b2Vec2(0, 10);
		b2World physWorld;
		const float32 physTimestep = 1.f / 128.f;
		double physAccumulator = 0.0;
		PhysContactListener phys_contact_listener;
		PhysDebugDraw phys_debug_draw;

		bool phys_draw_debug = false;


		//sf::Clock testTimer;
		//int testCounter = 0;
	};
}