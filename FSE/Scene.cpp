#include "Scene.h"
#include "Application.h"
#include "FSEObject/FSEObject.h"
#include "Lights/FSELightWorld.h"

namespace fse
{

	Scene::Scene(Application* application) : render_target_(nullptr), application_(application), phys_world_(phys_gravity_)
	{
		phys_world_.SetContactListener(&phys_contact_listener_);
		win_resize_signal_connection_ = application_->on_window_resized_.connect(this, &Scene::notifyResize);

		light_world_ = createFSEObject<FSELightWorld>().lock().get();
		processPendingSpawns();
	}

	Scene::Scene(Application* application, float pixel_meter_ratio) : Scene(application)
	{
		pixels_per_meter_ = pixel_meter_ratio;
		meters_per_pixel_ = 1.f / pixel_meter_ratio;
	}

	Scene::~Scene()
	{
		application_->on_window_resized_.disconnect(win_resize_signal_connection_);

		fse_objects_.clear();
		pending_object_removals_.clear();
		pending_object_spawns_.clear();

		std::wcout << "Deleting scene\n";
	}

	void Scene::setRenderTarget(sf::RenderTarget * renderTarget)
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

	std::weak_ptr<FSEObject> Scene::spawnFSEObject(std::shared_ptr<FSEObject> object)
	{
		pending_object_spawns_.push_back(object);
		return object;
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

	Application* Scene::getApplication() const
	{
		return application_;
	}

	FSELightWorld* Scene::getLightWorld() const
	{
		return light_world_;
	}


	b2World *Scene::getPhysWorld()
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
			fse_objects_.erase(it);
		}
	}

	void Scene::removeFSEObject(FSEObject * object)
	{
		auto it = std::find_if(fse_objects_.begin(), fse_objects_.end(), [&](const std::shared_ptr<FSEObject> & obj) {
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
			for (auto & object : pending_object_removals_)
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
			for (auto & object : pending_object_spawns_)
			{
				fse_objects_.push_back(object);
				fse_objects_.rbegin()->get()->spawn(spawn_count_++, this);
				z_order_changed_ = true;
			}
			pending_object_spawns_.clear();
		}
	}


	FSE_CHAI_REGISTER(Scene)
	{
		chai.add(chaiscript::user_type<Scene>(), "FSEScene");
		chai.add(chaiscript::fun(static_cast<bool(Scene::*)() const>(&Scene::isPaused)), "isPaused");
		chai.add(chaiscript::fun(static_cast<void(Scene::*)(bool)>(&Scene::setPaused)), "setPaused");
		chai.add(chaiscript::fun(static_cast<bool(Scene::*)() const>(&Scene::getPhysDrawDebug)), "getPhysDrawDebug");
		chai.add(chaiscript::fun(static_cast<void(Scene::*)(bool)>(&Scene::setPhysDrawDebug)), "setPhysDrawDebug");
		chai.add(chaiscript::fun(static_cast<FSELightWorld*(Scene::*)() const>(&Scene::getLightWorld)), "getLightWorld");
		chai.add(chaiscript::fun(static_cast<b2World*(Scene::*)()>(&Scene::getPhysWorld)), "getPhysWorld");
		chai.add(chaiscript::fun(static_cast<std::weak_ptr<FSEObject> (Scene::*)(std::shared_ptr<FSEObject>) > (&Scene::spawnFSEObject)), "spawnObject");
		chai.add(chaiscript::fun(([](Scene* scene)
		{
			return scene->getFSEObjects();
		})), "getObjects");
		chai.add(chaiscript::fun(([](Scene* scene, int id)
		{
			for (auto& object : *scene->getFSEObjects())
			{
				if (object->getID() == id)
					return object;
			}
			return static_cast<std::shared_ptr<FSEObject>>(nullptr);
		})), "getObjectWithId");
	}

}