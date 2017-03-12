#pragma once

#include <vector>
#include <Box2D/Box2D.h>

#include "Renderer.h"

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
	class Application;
	class FSEObject;
	class FSELightWorld;

	/*!
	 * \brief Scene where objects live in, get updated and rendered
	 */
	class Scene 
	{
	public:
		Scene(Application *application);
		~Scene();

		/*!
		 * Set rendertarget of scene
		 * \param renderTarget render target
		 */
		void setRenderTarget(sf::RenderTarget* renderTarget);
		sf::RenderTarget* getRenderTarget() const;

		/*!
		 * Update the scene
		 * \param deltaTime elapsed time in seconds
		 */
		void update(float deltaTime);
		/*!
		 * Issue draw calls
		 */
		void draw();

		/*!
		 * \brief Returns a ptr to all currently active objects \n
		 * 
		 * Iterating over all objects in a scene is a very expensive task,
		 * don't do it if you don't need to 
		 * \return Ptr to vector of currently active objects
		 */
		std::vector<std::unique_ptr<FSEObject>>* getFSEObjects();

		/*!
		 * \brief Notify of changed z-orders
		 * 
		 * re-sorts object vector \n 
		 * you shouldn't need to call this manually
		 */
		void notifyZOrderChanged();

		/*!
		 * \brief Spawn FSEObject into the scene \n
		 * 
		 * Usage: \n 
		 *		\code
		 *		auto obj = std::make_unique<YourObject>(scene);
		 *		scene->spawnFSEObject(std::move(obj));
		 *		\endcode
		 * 
		 * \param object unique_ptr to object
		 */
		void spawnFSEObject(std::unique_ptr<FSEObject> object);

		/*!
		 * \brief Create and spawn FSEObject
		 * Usage: \n
		 *  \code
		 *		scene->createFSEObject<YourObject>();
		 *	\endcode
		 */
		template<typename T>
		void createFSEObject()
		{
			std::unique_ptr<T> object = std::unique_ptr<T>(new T(this));
			pending_object_spawns_.push_back(std::move(object));
		}

		/*!
		* \brief Create and spawn FSEObject, connect slot to spawned signal \n
		* 
		* Usage: \n
		* \code
		*		scene->createFSEObject<YourObject>(slot);
		* \endcode
		* Spawned signal gets emitted after the object has spawned \see FSEObject, \see Signal \n
		* Note: ATTENTION: \n
		* It is NOT reccommended to store across multiple frames pointers to objects if you can't guarantee they will outlive you or live as long as you do.
		* \param slot Slot to call after object spawn
		*/
		template<typename T, typename SpawnedSlot>
		void createFSEObject(SpawnedSlot&& slot)
		{
			std::unique_ptr<T> object = std::unique_ptr<T>(new T(this));
			object->spawned_signal_.connect(slot);
			pending_object_spawns_.push_back(std::move(object));
		}

		/*!
		* \brief Create and spawn FSEObject at position \n 
		* 
		* Usage: \n
		* \code
		*		scene->createFSEObject<YourObject>(spawnpos);
		* \endcode
		* \param spawnPos spawn position in meters
		*/
		template<typename T>
		void createFSEObject(const sf::Vector2f spawnPos)
		{
			std::unique_ptr<T> object = std::unique_ptr<T>(new T(this, spawnPos));
			pending_object_spawns_.push_back(std::move(object));
		}

		/*!
		* \brief Create and spawn FSEObject at position, connect slot to spawned signal \n 
		* 
		* Usage: \n
		* \code
		*		scene->createFSEObject<YourObject>(spawnpos, slot);
		* \endcode
		* Spawned signal gets emitted after the object has spawned \see FSEObject, \see Signal \n
		* Note: ATTENTION: \n
		* It is NOT reccommended to store across multiple frames pointers to objects if you can't guarantee they will outlive you or live as long as you do.
		* \param spawnPos spawn position in meters
		* \param slot Slot to call after object spawn
		*/
		template<typename T, typename SpawnedSlot>
		void createFSEObject(const sf::Vector2f spawnPos, SpawnedSlot&& slot)
		{
			std::unique_ptr<T> object = std::unique_ptr<T>(new T(this, spawnPos));
			object->spawned_signal_.connect(slot);
			pending_object_spawns_.push_back(std::move(object));
		}

		void destroyFSEObject(FSEObject* FSEObject);

		bool isPaused() const;
		void setPaused(bool paused);

		bool getPhysDrawDebug() const;
		void setPhysDrawDebug(bool drawDebug);
		PhysDebugDraw& getPhysDebugDraw();

		/*!
		 * Notifies scene of window resize \n 
		 */
		void notifyResize();

		/*!
		 * \return Ptr to Application
		 */
		Application* getApplication() const;

		/*!
		 * \return Ptr to FSELightWorld
		 */
		FSELightWorld* getLightWorld() const;

		/*!
		 * Refer to Box2D docu
		 * \return Ptr to Box2D Physics world
		 */
		b2World *getPhysWorld();


	protected:
		void addFSEObject(std::unique_ptr<FSEObject> object);
		void removeFSEObject(std::unique_ptr<FSEObject> const & object);

		void removeFSEObject(FSEObject* FSEObject);

		void processPendingRemovals();
		void processPendingSpawns();


		sf::RenderTarget* render_target_;

		std::vector<std::unique_ptr<FSEObject> > fse_objects_; 

		std::list<std::unique_ptr<FSEObject> > pending_object_spawns_;
		std::list<FSEObject*> pending_object_removals_;


		std::unique_ptr<Renderer> renderer_;

	private:
		bool z_order_changed_ = false;
		int spawn_count_ = 0;

		bool is_paused_ = false;

		float elapsed_time_ = 0;

		Application *application_;
		Signal<>::Connection win_resize_signal_connection_;

		NetworkHandler* network_handler_ = nullptr;
		
		FSELightWorld* light_world_ = nullptr;

		const int32 phys_velocy_iters_ = 16;
		const int32 phys_pos_iters_ = 6;
		b2Vec2 phys_gravity_ = b2Vec2(0, 10);
		b2World phys_world_;
		const float32 phys_timestep_ = 1.f / 128.f;
		double phys_accumulator_ = 0.0;
		PhysContactListener phys_contact_listener_;
		PhysDebugDraw phys_debug_draw_;

		bool phys_draw_debug_ = false;

		friend class Serializer;

	};
}