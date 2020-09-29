#include "Scene.h"
#include "Application.h"
#include "FSEObject/FSEObject.h"
#include "Lights/FSELightWorld.h"
#include <v8pp/module.hpp>
#include <v8pp/class.hpp>
#include "FSEObject/ScriptObject.h"

#include "UUID.h"

namespace fse
{
	struct SceneObjectNotifyPacket
	{
		fse::Scene::PacketType type;
		std::string networkId;
		std::string className;
		std::string scriptClassName;
		uint32_t timestamp;
	};
}


sf::Packet& operator >>(sf::Packet& packet, fse::SceneObjectNotifyPacket& data)
{
	uint8_t temp_type;
	packet >> temp_type >> data.networkId >> data.className >> data.timestamp;
	data.type = static_cast<fse::Scene::PacketType>(temp_type);
	return packet;
}

namespace fse
{

	Scene::Scene()
        :
            PacketHandler(),
            render_target_(nullptr),
            win_resize_signal_connection_(
				Application::get()->on_window_resized_,
				Application::get()->on_window_resized_.connect(this, &Scene::notifyResize)
			),
            phys_world_(phys_gravity_)
	{
		phys_world_.SetContactListener(&phys_contact_listener_);

		light_world_ = createFSEObject<FSELightWorld>().lock();
		processPendingSpawns();
	}

	Scene::Scene(float pixel_meter_ratio) : Scene()
	{
		pixels_per_meter_ = pixel_meter_ratio;
		meters_per_pixel_ = 1.f / pixel_meter_ratio;
	}

	Scene::~Scene()
	{
		// Application::get()->on_window_resized_.disconnect(win_resize_signal_connection_);
		fse_objects_.clear();
		pending_object_removals_.clear();
		pending_object_spawns_.clear();

		std::wcout << "Deleting scene\n";
	}

	void Scene::setRenderTarget(sf::RenderTarget* renderTarget)
	{
		render_target_ = renderTarget;
		renderer_ = std::make_unique<Renderer>(render_target_);

		phys_debug_draw_ = PhysDebugDraw(*render_target_, pixels_per_meter_);
		phys_world_.SetDebugDraw(&phys_debug_draw_);


		light_world_->init(renderTarget);

		renderer_->setNormalTarget(light_world_->getNormalTarget());
		renderer_->setSpecularTarget(light_world_->getSpecularTarget());

	}

	sf::RenderTarget* Scene::getRenderTarget() const
	{
		return render_target_;
	}


	void Scene::update(float deltaTime)
	{
		netUpdate(deltaTime);
		processPendingSpawns();

		if (is_paused_)
			deltaTime = 0.0f;

		phys_accumulator_ += deltaTime;

		while (phys_accumulator_ >= phys_timestep_)
		{
			phys_world_.Step(phys_timestep_, phys_velocy_iters_, phys_pos_iters_);
			phys_accumulator_ -= phys_timestep_;
		}

		for (auto it = fse_objects_.rbegin(); it != fse_objects_.rend(); ++it)
		{
			it->get()->update(deltaTime);
		}

		processPendingRemovals();
	}

    void Scene::netUpdate(float deltaTime)
    {
		PacketHandler::netUpdate(deltaTime);
		for (auto& packet : all_packets_)
		{
			if (!packet.endOfPacket()) // TODO: refactor receive packet API
			{
				SceneObjectNotifyPacket packetData;
				packet >> packetData;

				if (packetData.type == PacketType::SpawnedOnHost && !Application::get()->isServer())
				{
					if (packetData.className == "fse::ScriptObject")
					{
						auto isolate = v8::Isolate::GetCurrent();
						v8::HandleScope handle_scope(isolate);
						auto ctx = isolate->GetCurrentContext();
						auto ctorFun = ctx->Global()->Get(ctx, v8pp::to_v8(isolate, "fse")).ToLocalChecked().As<v8::Object>()
							->Get(ctx, v8pp::to_v8(isolate, "Scene")).ToLocalChecked().As<v8::Object>()
							->Get(ctx, v8pp::to_v8(isolate, "constructScriptObject")).ToLocalChecked().As<v8::Function>();
						v8::Local<v8::Value> args[] = { v8pp::to_v8(isolate, packetData.scriptClassName) };
						auto maybeObject = (
							ctorFun->Call(ctx, v8::Undefined(isolate), 1, args)
						);
						if (maybeObject.IsEmpty())
						{
							std::cout << "object " << packetData.className << " was not constructable!\n";
						} else
						{
							auto nativeObject = (
								v8pp::from_v8<std::shared_ptr<ScriptObject>>(isolate, maybeObject.ToLocalChecked())
							);
							nativeObject->setNetworkId(packetData.networkId);
							pending_object_spawns_.push_back(nativeObject);
						}

					}
				    else
					{
						rttr::type type = rttr::type::get_by_name(packetData.className);
						auto ctor = type.get_constructor({});
						if (ctor.is_valid())
						{
							auto object = ctor.invoke().get_value<std::shared_ptr<fse::FSEObject>>();
							object->setNetworkId(packetData.networkId);
							pending_object_spawns_.push_back(object);
						}
						else {
							std::cout << "object " << packetData.className << " was not constructable!\n";
						}
					}					
				}
			}
		}
    }

    void Scene::draw()
	{
		if (z_order_changed_)
		{
			std::sort(fse_objects_.begin(), fse_objects_.end(),
				[](const std::shared_ptr<FSEObject>& a, const std::shared_ptr<FSEObject>& b) {

					return a->getZOrder() < b->getZOrder();

				});

			z_order_changed_ = false;
		}

		light_world_->updateView();

		renderer_->render(fse_objects_);

		if (phys_draw_debug_)
		{
			phys_world_.DrawDebugData();
		}
	}

	const std::vector<std::shared_ptr<FSEObject> >* Scene::getFSEObjects() const
	{
		return &fse_objects_;
	}

	void Scene::notifyZOrderChanged()
	{
		z_order_changed_ = true;
	}

	void Scene::destroyFSEObject(FSEObject* object)
	{
		pending_object_removals_.push_back(object);
	}

	bool Scene::isPaused() const
	{
		return is_paused_;
	}

	void Scene::setPaused(bool paused)
	{
		this->is_paused_ = paused;
	}

	bool Scene::getPhysDrawDebug() const
	{
		return phys_draw_debug_;
	}

	void Scene::setPhysDrawDebug(bool drawDebug)
	{
		phys_draw_debug_ = drawDebug;
	}

	PhysDebugDraw& Scene::getPhysDebugDraw()
	{
		return phys_debug_draw_;
	}

	void Scene::notifyResize()
	{
		sf::View view = render_target_->getView();
		sf::IntRect viewPort = render_target_->getViewport(view);
		view.setSize(sf::Vector2f(static_cast<float>(viewPort.width), static_cast<float>(viewPort.height)));
		render_target_->setView(view);
	}


	std::shared_ptr<FSELightWorld> Scene::getLightWorld() const
	{
		return light_world_;
	}


	b2World* Scene::getPhysWorld()
	{
		return &phys_world_;
	}

	float Scene::getPixelsPerMeter() const
	{
		return pixels_per_meter_;
	}

	float Scene::getMetersPerPixel() const
	{
		return meters_per_pixel_;
	}

	void Scene::removeFSEObject(std::shared_ptr<FSEObject> const& object)
	{
		auto it = std::find(fse_objects_.begin(), fse_objects_.end(), object);
		if (it != fse_objects_.end())
		{
			it->get()->onDespawn();
			it->get()->despawn();
			fse_objects_.erase(it);
		}
	}

	void Scene::removeFSEObject(FSEObject* object)
	{
		auto it = std::find_if(fse_objects_.begin(), fse_objects_.end(), [&](const std::shared_ptr<FSEObject>& obj) {
			return obj.get() == object;
			});
		if (it != fse_objects_.end())
		{
			it->get()->onDespawn();
			it->get()->despawn();
			fse_objects_.erase(it);
		}
	}

	void Scene::processPendingRemovals()
	{
		if (!pending_object_removals_.empty())
		{
			for (auto& object : pending_object_removals_)
			{
				removeFSEObject(object);
			}
			pending_object_removals_.clear();
		}

	}

	void Scene::processPendingSpawns()
	{
		if (!pending_object_spawns_.empty())
		{
			for (auto& object : pending_object_spawns_)
			{
				fse_objects_.push_back(object);
				fse_objects_.rbegin()->get()->spawn(spawn_count_++, this);
				z_order_changed_ = true;
			}
			pending_object_spawns_.clear();
		}
	}

    void Scene::handleNetworkSpawn(std::shared_ptr<FSEObject> const& object) const
    {
		rttr::type type = rttr::type::get(*object.get());
		if (type.get_name() == "fse::ScriptObject")
		{
			std::cout << "Object is ScriptObject; Sending ScriptPacket!\n";
			if (object->getNetworkId().empty())
				object->setNetworkId(uuid::v4());
			sf::Packet packet;
			packet
				<< getNetworkId().data()
				<< static_cast<uint8_t>((Application::get()->isServer() ? PacketType::SpawnedOnHost : PacketType::SpawnedOnClient))
				<< object->getNetworkId().data()
				<< type.get_name().data()
				<< std::dynamic_pointer_cast<ScriptObject>(object)->scriptClassName;
				;
			// TODO: refactor sendPacketApi
			sendTcpPacket(packet);
			return;
		}
		if (!type.get_constructor({}).is_valid())
		{
			std::cout << "Object is not constructable\n";
			return;
		}
		if (object->getNetworkId().empty())
			object->setNetworkId(uuid::v4());
		sf::Packet packet;
		packet
			<< getNetworkId().data()
		    << static_cast<uint8_t>((Application::get()->isServer() ? PacketType::SpawnedOnHost : PacketType::SpawnedOnClient))
		    << object->getNetworkId().data()
		    << type.get_name().data()
		    << "" // empty script class name
	        ;
		// TODO: refactor sendPacketApi
		sendTcpPacket(packet);
    }

    FSE_V8_REGISTER(Scene)
	{
		v8::HandleScope handle_scope(isolate);

		v8pp::class_<Scene> Scene_class(isolate);
		// Scene_class.inherit<PacketHandler>();
		Scene_class.function("isPaused", &Scene::isPaused);
		Scene_class.function("setPaused", &Scene::setPaused);
		//Scene_class.property("paused", &Scene::isPaused, &Scene::setPaused);
		
		Scene_class.function("getPhysDrawDebug", &Scene::getPhysDrawDebug);
		Scene_class.function("setPhysDrawDebug", &Scene::setPhysDrawDebug);
		Scene_class.property("physDrawDebug", &Scene::getPhysDrawDebug, &Scene::setPhysDrawDebug);
		Scene_class.function("getLightWorld", &Scene::getLightWorld);
		Scene_class.function("getPhysWorld", &Scene::getPhysWorld);
		Scene_class.function("getPixelsPerMeter", &Scene::getPixelsPerMeter);
		Scene_class.function("getMetersPerPixel", &Scene::getMetersPerPixel);
		Scene_class.function("getRenderTarget", &Scene::getRenderTarget);
		//Scene_class.function("spawnObject", static_cast<std::weak_ptr<FSEObject> (Scene::*)(std::shared_ptr<FSEObject>)>(&Scene::spawnFSEObject));
		Scene_class.function("spawnObject", [](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				v8::Isolate* isolate = args.GetIsolate();
				const auto scene = v8pp::from_v8<Scene*>(isolate, args.This());
				auto object = v8pp::from_v8<std::shared_ptr<fse::FSEObject>>(isolate, args[0]);

				if (object == nullptr)
				{
					auto jsObject = args[0].As<v8::Object>();
					if (jsObject->Has(isolate->GetCurrentContext(), v8pp::to_v8(isolate, "native")).FromMaybe(false))
					{
						auto maybeNative = jsObject->Get(
							isolate->GetCurrentContext(),
							v8pp::to_v8(isolate, "native")
						);
						if (!maybeNative.IsEmpty())
						    object = v8pp::from_v8<std::shared_ptr<fse::FSEObject>>(isolate, maybeNative.ToLocalChecked());
					}
					if (object == nullptr)
					{
						throw std::exception("Expected FSEObjectNative");
					}
				}
				if (args.Length() >= 2 && args[1]->IsFunction())
				{
					auto callback = args[1].As<v8::Function>();
					if (!callback.IsEmpty())
						object->spawned_signal_.connectJs(callback);
				}
				auto weak_obj = static_cast<std::weak_ptr<FSEObject>>(scene->spawnFSEObject(object));
			});
		Scene_class.function("getObjects", [](v8::FunctionCallbackInfo<v8::Value> const& args)
			{
				v8::Isolate* isolate = args.GetIsolate();
				const auto scene = v8pp::from_v8<Scene*>(isolate, args.This());
				std::vector<std::shared_ptr<fse::FSEObject>> result;
				result.reserve(scene->getFSEObjects()->size());
				for (auto& object : *scene->getFSEObjects())
				{
					result.push_back(object);
				}
				return result;
			});
		Scene_class.function("getObjectWithId", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			v8::Isolate* isolate = args.GetIsolate();
			const auto scene = v8pp::from_v8<Scene*>(isolate, args.This());
			const auto id = v8pp::from_v8<int>(isolate, args[0]);
			for (auto& object : *scene->getFSEObjects())
			{
				if (object->getID() == id)
					return std::weak_ptr<FSEObject>(object);
			}
			return std::weak_ptr<FSEObject>();
		});
		module.class_("Scene", Scene_class);
	}

}