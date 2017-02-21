#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>

#include "../Scene.h"

#include "../Input.h"

namespace fse
{
	
	class FSEObject
	{
	public:

		typedef Signal<FSEObject*> SpawnedSignal;

		FSEObject(Scene *scene);
		FSEObject(Scene *scene, const sf::Vector2f spawnPos);
		virtual ~FSEObject();


		virtual void update(float deltaTime) = 0;

		virtual void draw(sf::RenderTarget& target) = 0;
		virtual void drawNormals(sf::RenderTarget& target);
		virtual void drawSpecular(sf::RenderTarget& target);

		virtual void spawned() = 0;

		virtual void setPosition(const sf::Vector2f position);
		virtual sf::Vector2f getPosition();

		virtual sf::FloatRect GetAABBs() const;

		virtual void BeginContact(FSEObject* otherObject, b2Contact* contact);
		virtual void EndContact(FSEObject* otherObject, b2Contact* contact);
		virtual void PreSolve(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold);
		virtual void PostSolve(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse);

		Scene* getScene();

		void spawn(int id);
		virtual bool destroy();

		int getID();
		int getZOrder() const;
		bool isPendingKill() const;
		void setTimedKill();

		Scene *scene_;

	protected:
		void setZOrder(int ZOrder);
		std::vector<std::unique_ptr<FSEObject> >* getSceneFSEObjects() const;

		sf::Drawable *drawable_;

		/// position in METERS
		sf::Vector2f position_;

		fse::Input* input_;

	private:
		int id_ = -1;
		int z_order_ = 0;
		bool is_pending_kill_ = false;
		bool pending_timed_kill_ = false;

	PUBLIC_SIGNALS:
		SpawnedSignal spawned_signal_;



	};
}

