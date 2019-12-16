#include "FSEObject.h"
#include "../Application.h"
#include "../Component/Component.h"

#include <rttr/registration>
#include <v8pp/module.hpp>
#include <v8pp/class.hpp>

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
		const sf::View v = scene_->getRenderTarget()->getView();
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


	const std::vector<std::shared_ptr<Component>>* FSEObject::getComponents() const
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
		spawned();
		spawned_signal_(this);
		spawned_signal_.disconnectAll();
	}

	void FSEObject::despawn()
	{
		components_.clear();
		scene_ = nullptr;
		input_ = nullptr;
		id_ = -1;
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

	//chaiscript::ChaiScript* FSEObject::getChai() const
	//{
	//	return scene_->getApplication()->getChai();
	//}

	std::weak_ptr<Component> FSEObject::attachComponent(std::shared_ptr<Component> component)
	{
		auto it = std::find_if(components_.begin(), components_.end(), [&](const std::shared_ptr<Component> & obj) {
			return obj.get() == component.get();
		});
		if (it != components_.end())
		{
			return std::weak_ptr<Component>();
		}
		components_.push_back(component);
		(*components_.rbegin())->attachToObject(this);
		return (*components_.rbegin());
	}

	std::shared_ptr<Component> FSEObject::detachComponent(Component*  component)
	{
		auto it = std::find_if(components_.begin(), components_.end(), [&](const std::shared_ptr<Component> & obj) {
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


	FSE_V8_REGISTER_BASE(FSEObject)
	{

		v8::HandleScope handle_scope(isolate);
		v8pp::class_<FSEObject> FSEObject_class(isolate);
		FSEObject_class.auto_wrap_objects(true);
		FSEObject_class.function("getID", static_cast<int (FSEObject::*)() const>(&FSEObject::getID));
		FSEObject_class.function("getZOrder", static_cast<int (FSEObject::*)() const>(&FSEObject::getZOrder));
		FSEObject_class.function("setZOrder", static_cast<void(FSEObject::*)(int)>(&FSEObject::setZOrder));
		FSEObject_class.function("isPendingKill", static_cast<bool(FSEObject::*)() const>(&FSEObject::isPendingKill));
		FSEObject_class.function("setTimedKill", static_cast<void(FSEObject::*)()>(&FSEObject::setTimedKill));
		FSEObject_class.function("getPosition", static_cast<sf::Vector2f(FSEObject::*)()>(&FSEObject::getPosition));
		//FSEObject_class.function("setPosition", static_cast<void(FSEObject::*)(sf::Vector2f position)>(&FSEObject::setPosition));
		FSEObject_class.function("setPosition", [](v8::FunctionCallbackInfo<v8::Value> const& args) //[](FSEObject* object, float x, float y)
			{
				// TODO: accept sf::Vector2<T>
				v8::Isolate* isolate = args.GetIsolate();
				auto object = v8pp::from_v8<fse::FSEObject*>(isolate, args.This());
				object->setPosition({ v8pp::from_v8<float>(isolate, args[0]), v8pp::from_v8<float>(isolate, args[1]) });
			});
		FSEObject_class.function("getAABBs", static_cast<sf::FloatRect(FSEObject::*)() const>(&FSEObject::GetAABBs));
		FSEObject_class.function("destroy", static_cast<bool(FSEObject::*)()>(&FSEObject::destroy));
		FSEObject_class.function("getScene", static_cast<Scene * (FSEObject::*)() const>(&FSEObject::getScene));
		FSEObject_class.function("getInput", [](v8::FunctionCallbackInfo<v8::Value> const& args) //[](const FSEObject* object)
			{
				v8::Isolate* isolate = args.GetIsolate();
				const auto object = v8pp::from_v8<fse::FSEObject*>(isolate, args.This());
				return object->input_;
			});
		FSEObject_class.function("attachComponent", static_cast<std::weak_ptr<Component>(FSEObject::*)(std::shared_ptr<Component>)>(&FSEObject::attachComponent));
		FSEObject_class.function("detachComponent", static_cast<std::shared_ptr<Component>(FSEObject::*)(Component*)>(&FSEObject::detachComponent));
		FSEObject_class.function("getComponents", [](v8::FunctionCallbackInfo<v8::Value> const& args) //[](const FSEObject* object)
			{
				v8::Isolate* isolate = args.GetIsolate();
				const auto object = v8pp::from_v8<fse::FSEObject*>(isolate, args.This());
				std::vector<std::weak_ptr<Component>> result;
				result.reserve(object->components_.size());
				for (auto& component : object->components_)
				{
					result.emplace_back(component);
				}
				return result;
			});


		
		//FSEObject_class.function(chaiscript::fun(([](const FSEObject* object) {
		//	if (object == nullptr)
		//		return std::string({ 'n', 'u', 'l', 'l' }); funny written because my cheatsheet generator script is crap
		//		return object->get_type().get_name().to_string();
		//	})), "getTypeName");
		//FSEObject_class.function(chaiscript::fun(([](const FSEObject* object) {
		//	if (object == nullptr)
		//		return std::string({ 'n', 'u', 'l', 'l' });
		//	return object->get_type().get_name().to_string();
		//	})), "type_name");
		module.class_("FSEObject", FSEObject_class);
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
		.property("components_", &FSEObject::components_)
		.method("destroy", &FSEObject::destroy)
		;

}