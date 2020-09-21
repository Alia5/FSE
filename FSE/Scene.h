#pragma once

#include <vector>
#include <Box2D/Box2D.h>

#include "Renderer.h"

#include "Signals.h"
#include "PhysContactListener.h"
#include "PhysDebugDraw.h"
#include "NetworkHandler.h"
#include "FSEV8Lib.h"

constexpr auto FSE_RADTODEG = 57.2957795f;
constexpr auto FSE_DEGTORAD = 0.01745329f;
//#define FSE_PXMRATIO = 25.f
//#define FSE_PIXELS_PER_METER 25.f
//#define FSE_METERS_PER_PIXEL (1.f/25.f)

namespace fse
{
	class FSEObject;
	class FSELightWorld;

	/*!
	 * \brief Scene where objects live in, get updated and rendered
	 */
	class Scene 
	{
	public:
		explicit Scene();
		explicit Scene(float pixel_meter_ratio);
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
		const std::vector<std::shared_ptr<FSEObject>>* getFSEObjects() const;

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
		 *		auto obj = std::make_shared<YourObject>();
		 *		scene->spawnFSEObject(obj);
		 *		\endcode
		 * 
		 * \param object shared_ptr to object
		 */
		std::weak_ptr<FSEObject> spawnFSEObject(std::shared_ptr<FSEObject> object);

		/*!
		 * \brief Spawn FSEObject into the scene \n
		 *
		 * Usage: \n
		 *		\code
		 *		auto obj = std::make_shared<YourObject>();
		 *		scene->spawnFSEObject(obj, [](FSEObject* object){//doStuff});
		 *		\endcode
		 *
		 * \param object shared_ptr to object
		 * \param slot callback when object is spawned
		 */
		template<typename SpawnedSlot>
		void spawnFSEObject(std::shared_ptr<FSEObject> object, SpawnedSlot&& slot);

		/*!
		 * \brief Create and spawn FSEObject
		 * Usage: \n
		 *  \code
		 *		scene->createFSEObject<YourObject>();
		 *	\endcode
		 */
		template<typename T>
		std::weak_ptr<T> createFSEObject()
		{
			std::shared_ptr<T> object = std::make_shared<T>();
			pending_object_spawns_.push_back(object);
			return  object;
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
		std::weak_ptr<T> createFSEObject(SpawnedSlot&& slot)
		{
			std::shared_ptr<T> object = std::make_shared<T>();
			object->spawned_signal_.connect(slot);
			pending_object_spawns_.push_back(object);
			return  object;
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
		std::weak_ptr<T> createFSEObject(const sf::Vector2f spawnPos)
		{
			std::shared_ptr<T> object = std::make_shared<T>(spawnPos);
			pending_object_spawns_.push_back(object);
			return  object;
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
		std::weak_ptr<T> createFSEObject(const sf::Vector2f spawnPos, SpawnedSlot&& slot)
		{
			std::shared_ptr<T> object = std::make_shared<T>(spawnPos);
			object->spawned_signal_.connect(slot);
			pending_object_spawns_.push_back(object);
			return  object;
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
		 * \return Ptr to FSELightWorld
		 */
		std::shared_ptr<FSELightWorld> getLightWorld() const;

		/*!
		 * Refer to Box2D docu
		 * \return Ptr to Box2D Physics world
		 */
		b2World *getPhysWorld();

		float getPixelsPerMeter() const;
		float getMetersPerPixel() const;

	protected:
		void removeFSEObject(std::shared_ptr<FSEObject> const & object);

		void removeFSEObject(FSEObject* object);

		void processPendingRemovals();
		void processPendingSpawns();


		sf::RenderTarget* render_target_;

		std::vector<std::shared_ptr<FSEObject> > fse_objects_; 

		std::list<std::shared_ptr<FSEObject> > pending_object_spawns_;
		std::list<FSEObject*> pending_object_removals_;


		std::unique_ptr<Renderer> renderer_;

		/*!
		*	The ratio how many pixels occupy a meter in game coordinates
		*/
		float pixels_per_meter_ = 25.f;
		/*!
		*	The ratio how many meters in game coordinates occupy a meter in pixels
		*/
		float meters_per_pixel_ = (1.f / 25.f);

	private:
		bool z_order_changed_ = false;
		int spawn_count_ = 0;

		bool is_paused_ = false;

		float elapsed_time_ = 0;

		Signal<>::ScopedConnection win_resize_signal_connection_;

		NetworkHandler* network_handler_ = nullptr;
		
		std::shared_ptr<FSELightWorld> light_world_ = nullptr;

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

		FSE_V8_ENABLE(Scene);
		FSE_V8_REGISTRATION_FRIEND

	};
}