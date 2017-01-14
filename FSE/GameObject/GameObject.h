#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>

#include "../Scene.h"

#include "../Input.h"

namespace fse
{
	
	class GameObject
	{
	public:

		typedef Signal<GameObject*> SpawnedSignal;

		GameObject(Scene *scene);
		GameObject(Scene *scene, const sf::Vector2f spawnPos);
		virtual ~GameObject();


		virtual void update(float deltaTime) = 0;

		virtual void draw(sf::RenderTarget& target) = 0;

		virtual void spawned() = 0;

		virtual void setPosition(const sf::Vector2f position);
		virtual sf::Vector2f getPosition();

		virtual void BeginContact(GameObject* otherObject, b2Contact* contact);
		virtual void EndContact(GameObject* otherObject, b2Contact* contact);
		virtual void PreSolve(GameObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold);
		virtual void PostSolve(GameObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse);

		template<typename T>
		static std::unique_ptr<T> createObject(Scene* scene)
		{
			return std::unique_ptr<T>(new T(scene));
		}

		template<typename T>
		static std::unique_ptr<T> createObject(Scene* scene, const sf::Vector2f startPos)
		{
			return std::unique_ptr<T>(new T(scene, startPos));
		}

		Scene* getScene();

		void spawn(int id);
		virtual bool destroy();

		int getID();
		int getZOrder() const;
		bool isPendingKill() const;
		void setTimedKill();

	protected:
		sf::Drawable *m_Drawable;

		void setZOrder(int ZOrder);

		std::vector<std::unique_ptr<GameObject> >* getSceneGameObjects() const;

		/// position in METERS
		sf::Vector2f m_position;

		fse::Input* m_input;

	private:
		int m_ID = -1;
		int m_ZOrder = 0;
		bool m_isPendingKill = false;
		bool m_pendingTimedKill = false;

		Scene *m_Scene;

	PUBLIC_SIGNALS:
		SpawnedSignal spawnedSignal;



	};
}

