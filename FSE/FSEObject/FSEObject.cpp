#include "FSEObject.h"
#include "../Application.h"
#include "../Component/Component.h"

#include <rttr/registration>

namespace fse
{

	FSEObject::FSEObject(Scene *scene) : position_(sf::Vector2f(0,0))
	{
		scene_ = scene;
		input_ = scene->getApplication()->getInput();
	}

	FSEObject::FSEObject(Scene* scene, const sf::Vector2f spawnPos) : FSEObject(scene)
	{
		position_ = spawnPos;
	}


	FSEObject::~FSEObject()
	{
		
	}

	void FSEObject::drawNormals(sf::RenderTarget& target)
	{
	}

	void FSEObject::drawSpecular(sf::RenderTarget& target)
	{
	}

	void FSEObject::setPosition(const sf::Vector2f position)
	{
		position_ = position;
	}

	sf::Vector2f FSEObject::getPosition()
	{
		return position_;
	}

	sf::FloatRect FSEObject::GetAABBs() const
	{
		//Always draw if not overridden...
		sf::View v = scene_->getRenderTarget()->getView();
		return sf::FloatRect(std::trunc(v.getCenter().x), std::trunc(v.getCenter().y), 1, 1);
	}

	void FSEObject::BeginContact(FSEObject* otherObject, b2Contact* contact)
	{
	}

	void FSEObject::EndContact(FSEObject* otherObject, b2Contact* contact)
	{
	}

	void FSEObject::PreSolve(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold)
	{
	}

	void FSEObject::PostSolve(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
	{
	}


	int FSEObject::getZOrder() const
	{
		return z_order_;
	}

	bool FSEObject::isPendingKill() const
	{
		if (pending_timed_kill_)
			return true;
		return is_pending_kill_;
	}

	void FSEObject::setTimedKill()
	{
		pending_timed_kill_ = true;
	}


	Scene* FSEObject::getScene() const
	{
		return scene_;
	}

	void FSEObject::spawn(int id)
	{
		id_ = id;
		spawned();
		spawned_signal_(this);
		spawned_signal_.disconnectAll();
	}

	bool FSEObject::destroy()
	{
		if (!is_pending_kill_)
		{
			is_pending_kill_ = true;

			scene_->destroyFSEObject(this);

			return true;
		}
		return false;
	}

	int FSEObject::getID() const
	{
		return id_;
	}

	void FSEObject::setZOrder(int ZOrder)
	{
		z_order_ = ZOrder;
		scene_->notifyZOrderChanged();
	}

	std::vector<std::unique_ptr<FSEObject>>* FSEObject::getSceneFSEObjects() const
	{
		return scene_->getFSEObjects();
	}

	void FSEObject::updateComponents(float deltaTime)
	{
		for (auto& component : components_)
		{
			component->update(deltaTime);
		}
	}

	Component* FSEObject::attachComponent(std::unique_ptr<Component> component)
	{
		auto it = std::find_if(components_.begin(), components_.end(), [&](const std::unique_ptr<Component> & obj) {
			return obj.get() == component.get();
		});
		if (it != components_.end())
		{
			return nullptr;
		}
		component->attachToObject(this);
		components_.push_back(std::move(component));
		return (*components_.rbegin()).get();
	}

	bool FSEObject::detachComponent(Component*  component)
	{
		auto it = std::find_if(components_.begin(), components_.end(), [&](const std::unique_ptr<Component> & obj) {
			return obj.get() == component;
		});
		if (it == components_.end())
		{
			return false;
		}
		(*it)->detach();
		components_.erase(it);
		return true;
	}

	std::vector<Component*> FSEObject::getComponentsRttr()
	{
		std::vector<Component*> res;

		for (auto& comp : components_)
			res.push_back(comp.get());

		return res;
	}

	void FSEObject::setRttrComponentsRttr(std::vector<Component*> components)
	{
		for (auto& component : components)
		{
			auto it = std::find_if(components_.begin(), components_.end(), [&](const std::unique_ptr<Component> & obj) {
				return obj.get() == component;
			});
			if (it == components_.end())
				continue;

			rttr::type type = rttr::type::get(*component);
			rttr::instance instA = rttr::instance(component);

			rttr::instance instB = rttr::instance(*it);

			for (auto& prop : type.get_properties())
			{
				prop.set_value(instA, prop.get_value(instB));
			}

		}
	}
}

RTTR_REGISTRATION
{
	using namespace rttr;
	using namespace fse;

	registration::class_<FSEObject>("fse::FSEObject")
		.property_readonly("id_", &FSEObject::getID)
		.property("position_", &FSEObject::getPosition, &FSEObject::setPosition)
		.property("z_order_", &FSEObject::getZOrder, &FSEObject::setZOrder)
		.property_readonly("aabbs_", &FSEObject::GetAABBs)
		.property("components_", &FSEObject::getComponentsRttr, &FSEObject::setRttrComponentsRttr)
		.method("destroy", &FSEObject::destroy)
		;

}