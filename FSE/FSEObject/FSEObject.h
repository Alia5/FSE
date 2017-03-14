#pragma once

#include <SFML/Graphics.hpp>

#include <rttr/type>

#include <memory>
#include <iostream>

#include "../Scene.h"
#include "../Input.h"
#include "../Component/Component.h"


namespace fse
{
	/*!
	 * \brief Base Class for every Object that can live in a Scene
	 */
	class FSEObject
	{
	public:

		/*!
		* \brief Signal gets emitted upon object spawn \n
		*
		* Signal gets emitted upon object spawn when using \code Scene::createFSEObject() \endcode with according overloads \n
		* \see FSEObject, \see Signal \n
		* Connections get disconnected and cleaned up after call \n
		* Note: ATTENTION: \n
		* It is NOT reccommended to store across multiple frames pointers to objects if you can't guarantee they will outlive you or live as long as you do.
		* Args: Ptr to spawned FSEObject (this)
		*/
		using SpawnedSignal = Signal<FSEObject*>;

		FSEObject(Scene *scene);
		FSEObject(Scene *scene, const sf::Vector2f spawnPos);
		virtual ~FSEObject();

		/*!
		 * Called once per frame \n 
		 * Used to update the object
		 * \param deltaTime elapsed time in seconds
		 */
		virtual void update(float deltaTime);

		/*!
		* Called once per frame, except when AABBs are out of view bounds \n 
		* Used to draw the diffuse of the object
		* \param target reference to the RenderTarget
		*/
		virtual void draw(sf::RenderTarget& target);
		/*!
		* Called once per frame, except when AABBs are out of view bounds \n 
		* Used to draw the normal map of the object
		* \param target reference to the RenderTarget
		*/
		virtual void drawNormals(sf::RenderTarget& target);
		/*!
		* Called once per frame, except when AABBs are out of view bounds \n 
		* Used to draw the specular map of the object
		* \param target reference to the RenderTarget
		*/
		virtual void drawSpecular(sf::RenderTarget& target);

		/*!
		 * Called upon object spawn \n 
		 * Before spawnsignal is emitted
		 */
		virtual void spawned() = 0;

		/*!
		 * Sets the object postion (in meters)
		 * \param position New Position 
		 */
		virtual void setPosition(const sf::Vector2f position);
		/*!
		* Returns the object postion (in meters)
		* \return object position
		*/
		virtual sf::Vector2f getPosition();

		/*!
		* \brief Returns AABBs of the object (in PIXELS).
		* 
		* Returns Axis Aligned Bounding Box of the object in PIXELS \n 
		* Overwrite to not get draw calls once outside view.
		* \return object AABBs
		*/
		virtual sf::FloatRect GetAABBs() const;

		/*!
		 * \brief Called on Contact begin
		 * Refer to Box2D doc
		 * 
		 * \param otherObject Ptr to other object
		 * \param contact Ptr to b2Contact class
		 */
		virtual void BeginContact(FSEObject* otherObject, b2Contact* contact);
		/*!
		* \brief Called on Contact end
		* Refer to Box2D doc
		* \param otherObject Ptr to other object
		* \param contact Ptr to b2Contact class
		*/
		virtual void EndContact(FSEObject* otherObject, b2Contact* contact);
		/*!
		* \brief Called before solving collision
		* Refer to Box2D doc
		*
		* \param otherObject Ptr to other object
		* \param contact Ptr to b2Contact class
		* \param oldManifold Ptr to old contact manifold
		*/
		virtual void PreSolve(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold);
		/*!
		* \brief Called after solving collision
		* Refer to Box2D doc
		*
		* \param otherObject Ptr to other object
		* \param contact Ptr to b2Contact class
		* \param impulse Ptr to contact impulse
		*/
		virtual void PostSolve(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse);

		/*!
		 * Returns Ptr to scene the object lives in
		 * \return scene
		 */
		Scene* getScene() const;

		/*!
		 * Called before object spawn
		 * \param id object creation ID
		 */
		void spawn(int id);
		/*!
		 * \brief Marks the object for destruction
		 * The Object will get destroyed after the current frame \n 
		 * Use \see setTimedKill() to mark for destruction if you don't want the object to be destroyed after the current frame
		 * 
		 * \return true if object is succesfully marked for destruction / false if already marked
		 */
		virtual bool destroy();

		/*!
		 * \return object creation ID
		 */
		int getID() const;
		/*!
		 * gets z-order of the object \n
		 * Higher values are further in front
		 * \return z-order
		 */
		int getZOrder() const;
		/*!
		 * \return true if the object is marked for destruction
		 */
		bool isPendingKill() const;

		/*!
		 * Mark object for destruction without destroying it yet 
		 */
		void setTimedKill();

	protected:
		/*!
		 * Set z-order of object
		 * \param ZOrder z-order
		 */
		void setZOrder(int ZOrder);
		/*!
		 * \brief Returns Ptr to scene's vector of currently active FSEObjects \n 
		 * Use when you for some odd reason need to query ALL objects, otherwise don't
		 */
		std::vector<std::unique_ptr<FSEObject> >* getSceneFSEObjects() const;

		/*!
		 * \brief update all currently attached components
		 */
		void updateComponents(float deltaTime);

		/*!
		* \brief Calls callback for all components - Called on Contact begin
		* Refer to Box2D doc
		*
		* \param otherObject Ptr to other object
		* \param contact Ptr to b2Contact class
		*/
		virtual void BeginContactComponents(FSEObject* otherObject, b2Contact* contact);
		/*!
		* \brief Calls callback for all components - Called on Contact end
		* Refer to Box2D doc
		* \param otherObject Ptr to other object
		* \param contact Ptr to b2Contact class
		*/
		virtual void EndContactComponents(FSEObject* otherObject, b2Contact* contact);
		/*!
		* \brief Calls callback for all components - Called before solving collision
		* Refer to Box2D doc
		*
		* \param otherObject Ptr to other object
		* \param contact Ptr to b2Contact class
		* \param oldManifold Ptr to old contact manifold
		*/
		virtual void PreSolveComponents(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold);
		/*!
		* \brief Calls callback for all components -  Called after solving collision
		* Refer to Box2D doc
		*
		* \param otherObject Ptr to other object
		* \param contact Ptr to b2Contact class
		* \param impulse Ptr to contact impulse
		*/
		virtual void PostSolveComponents(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse);

		/*!
		 * \brief attach a component to this object.
		 * 
		 * \returns ptr to component, nullptr on failure
		 */
		Component* attachComponent(std::unique_ptr<Component>  component);

		/*!
		* \brief detach a component from this object.
		* \returns detachment success
		*/
		bool detachComponent(Component*  component);

		/*!
		 * \brief Vector of attached components.
		 */
		std::vector<std::unique_ptr<Component> > components_;

		/*!
		 * Ptr to scene the object lives in.
		 */
		Scene *scene_;

		/*!
		 * position in meters.
		 */
		sf::Vector2f position_;

		/*!
		 * Ptr to input handling class.
		 */
		fse::Input* input_;

	private:
		int id_ = -1;
		int z_order_ = 0;
		bool is_pending_kill_ = false;
		bool pending_timed_kill_ = false;

		std::vector<Component*> getComponentsRttr();
		void setRttrComponentsRttr(std::vector<Component*> components);

		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND


	PUBLIC_SIGNALS:
		/*!
		* \brief Signal gets emitted upon object spawn \n
		*
		* Signal gets emitted upon object spawn when using \code Scene::createFSEObject() \endcode with according overloads \n
		* \see FSEObject, \see Signal \n
		* Connections get disconnected and cleaned up after call \n
		* Note: ATTENTION: \n
		* It is NOT reccommended to store across multiple frames pointers to objects if you can't guarantee they will outlive you or live as long as you do.
		* Args: Ptr to spawned FSEObject (this)
		*/
		SpawnedSignal spawned_signal_;


	};
}

