#include "FSEObject.h"
#include "../Application.h"
#include "../Component/Component.h"

#include <rttr/registration>

namespace fse
{

	FSEObject::FSEObject() : FSEObject(sf::Vector2f(0, 0))
	{
	}


	FSEObject::FSEObject(const sf::Vector2f spawnPos): scene_(nullptr), input_(nullptr)
	{
		position_ = spawnPos;
	}


	void FSEObject::update(float deltaTime)
	{
		updateComponents(deltaTime);
	}

	void FSEObject::draw(sf::RenderTarget& target)
	{
	}

	void FSEObject::drawNormals(sf::RenderTarget& target)
	{
	}

	void FSEObject::drawSpecular(sf::RenderTarget& target)
	{
	}

	bool FSEObject::isActive() const
	{
		return is_active_;
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
		BeginContactComponents(otherObject, contact);
	}

	void FSEObject::EndContact(FSEObject* otherObject, b2Contact* contact)
	{
		EndContactComponents(otherObject, contact);
	}

	void FSEObject::PreSolve(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold)
	{
		PreSolveComponents(otherObject, contact, oldManifold);
	}

	void FSEObject::PostSolve(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
	{
		PostSolveComponents(otherObject, contact, impulse);
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


	const std::vector<std::unique_ptr<Component>>* FSEObject::getComponents() const
	{
		return &components_;
	}


	Scene* FSEObject::getScene() const
	{
		return scene_;
	}

	void FSEObject::spawn(int id, Scene* scene)
	{
		scene_ = scene;
		input_ = scene->getApplication()->getInput();
		id_ = id;
		is_active_ = true;
		scene_->notifyZOrderChanged();
		spawned();
		spawned_signal_(this);
		spawned_signal_.disconnectAll();
	}

	bool FSEObject::destroy()
	{
		if (!is_pending_kill_)
		{
			is_active_ = false;
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
		if (scene_ != nullptr)
			scene_->notifyZOrderChanged();
	}

	const std::vector<std::shared_ptr<FSEObject>>* FSEObject::getSceneFSEObjects() const
	{
		return scene_->getFSEObjects();
	}

	void FSEObject::updateComponents(float deltaTime)
	{
		for (auto& component : components_)
			component->update(deltaTime);
	}

	void FSEObject::BeginContactComponents(FSEObject* otherObject, b2Contact* contact)
	{
		for (auto& component : components_)
			component->BeginContact(otherObject, contact);
	}

	void FSEObject::EndContactComponents(FSEObject* otherObject, b2Contact* contact)
	{
		for (auto& component : components_)
			component->EndContact(otherObject, contact);
	}

	void FSEObject::PreSolveComponents(FSEObject* otherObject, b2Contact* contact, const b2Manifold* oldManifold)
	{
		for (auto& component : components_)
			component->PreSolve(otherObject, contact, oldManifold);
	}

	void FSEObject::PostSolveComponents(FSEObject* otherObject, b2Contact* contact, const b2ContactImpulse* impulse)
	{
		for (auto& component : components_)
			component->PostSolve(otherObject, contact, impulse);
	}

	chaiscript::ChaiScript* FSEObject::getChai() const
	{
		return scene_->getApplication()->getChai();
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
		components_.push_back(std::move(component));
		(*components_.rbegin())->attachToObject(this);
		return (*components_.rbegin()).get();
	}

	std::unique_ptr<Component> FSEObject::detachComponent(Component*  component)
	{
		auto it = std::find_if(components_.begin(), components_.end(), [&](const std::unique_ptr<Component> & obj) {
			return obj.get() == component;
		});
		if (it == components_.end())
		{
			return nullptr;
		}
		it->get()->detach();
		auto res = std::move(*it);
		components_.erase(it);
		return res;
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