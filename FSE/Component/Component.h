#pragma once

#include <rttr/type>

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
		virtual void update(float deltaTime) = 0;

		FSEObject* getAttachedObject() const;
		void attachToObject(FSEObject* object);

		bool isAttached() const;

		void detach();

	protected:
		FSEObject* object_ = nullptr;

	private:
		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND
	};
}
