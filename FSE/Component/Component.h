#pragma once

#include <rttr/type>
#include <rttr/registration_friend>
#include "../FSEV8Lib.h"


struct b2Manifold;
class b2Contact;
struct b2ContactImpulse;

namespace fse
{
	class FSEObject;
	/*!
	 * \brief Base class for object components;
	 */
	class Component
	{
	public:
		Component();
		virtual ~Component() = default;
		/*!
		 * \brief Call every frame to update the component
		 * \param deltaTime elapsed time in seconds
		 */
		virtual void update(float deltaTime);

		virtual void onAttach();
		virtual void onDetach();



		std::weak_ptr<FSEObject> getAttachedObject() const;
		void attachToObject(std::shared_ptr<FSEObject> object);

		bool isAttached() const;

		void detach();

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

	protected:
		std::weak_ptr<FSEObject> object_;

	private:

		FSE_V8_ENABLE(Component);
		FSE_V8_REGISTRATION_FRIEND

		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND
	};
}
