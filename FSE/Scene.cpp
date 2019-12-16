#include "Scene.h"
#include "Application.h"
#include "FSEObject/FSEObject.h"
#include "Lights/FSELightWorld.h"
#include <v8pp/module.hpp>
#include <v8pp/class.hpp>

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

	template<typename SpawnedSlot>
	void Scene::spawnFSEObject(std::shared_ptr<FSEObject> object, SpawnedSlot&& slot)
	{
		object->spawned_signal_.connect(slot);
		pending_object_spawns_.push_back(object);
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


	FSE_V8_REGISTER(Scene)
	{
		//v8::EscapableHandleScope scope(isolate);
		v8::HandleScope handle_scope(isolate);

		v8pp::class_<Scene> Scene_class(isolate);
		Scene_class.function("isPaused", &Scene::isPaused);
		Scene_class.function("setPaused", &Scene::setPaused);
		Scene_class.function("getPhysDrawDebug", &Scene::getPhysDrawDebug);
		Scene_class.function("setPhysDrawDebug", &Scene::setPhysDrawDebug);
		Scene_class.function("getLightWorld", &Scene::getLightWorld);
		Scene_class.function("getPhysWorld", &Scene::getPhysWorld);
		Scene_class.function("getPixelsPerMeter", &Scene::getPixelsPerMeter);
		Scene_class.function("getMetersPerPixel", &Scene::getMetersPerPixel);
		Scene_class.function("getRenderTarget", &Scene::getRenderTarget);
		//Scene_class.function("spawnObject", static_cast<std::weak_ptr<FSEObject> (Scene::*)(std::shared_ptr<FSEObject>)>(&Scene::spawnFSEObject));
		Scene_class.function("getObjects", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			v8::Isolate* isolate = args.GetIsolate();
			const auto scene = v8pp::from_v8<Scene*>(isolate, args.This());
			return *scene->getFSEObjects();
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

		//module.class_("Scene", Scene_class);

		//// set bindings in global object as `mylib`
		//isolate->GetCurrentContext()->Global()->Set(isolate->GetCurrentContext(),
		//	v8::String::NewFromUtf8(isolate, "fse").ToLocalChecked(), module.new_instance());
		
		//chai.add(chaiscript::user_type<Scene>(), "FSEScene");
		//chai.add(chaiscript::fun(static_cast<bool(Scene::*)() const>(&Scene::isPaused)), "isPaused");
		//chai.add(chaiscript::fun(static_cast<void(Scene::*)(bool)>(&Scene::setPaused)), "setPaused");
		//chai.add(chaiscript::fun(static_cast<bool(Scene::*)() const>(&Scene::getPhysDrawDebug)), "getPhysDrawDebug");
		//chai.add(chaiscript::fun(static_cast<void(Scene::*)(bool)>(&Scene::setPhysDrawDebug)), "setPhysDrawDebug");
		//chai.add(chaiscript::fun(static_cast<FSELightWorld*(Scene::*)() const>(&Scene::getLightWorld)), "getLightWorld");
		//chai.add(chaiscript::fun(static_cast<b2World*(Scene::*)()>(&Scene::getPhysWorld)), "getPhysWorld");
		//chai.add(chaiscript::fun(static_cast<float(Scene::*)() const>(&Scene::getPixelsPerMeter)), "getPixelsPerMeter");
		//chai.add(chaiscript::fun(static_cast<float(Scene::*)() const>(&Scene::getMetersPerPixel)), "getMetersPerPixel");
		//chai.add(chaiscript::fun(static_cast<sf::RenderTarget*(Scene::*)() const>(&Scene::getRenderTarget)), "getRenderTarget");
		//chai.add(chaiscript::fun(static_cast<std::weak_ptr<FSEObject> (Scene::*)(std::shared_ptr<FSEObject>) > (&Scene::spawnFSEObject)), "spawnObject");
		//chai.add(chaiscript::fun(([](Scene* scene)
		//{
		//	return scene->getFSEObjects();
		//})), "getObjects");
		////chai.add(chaiscript::fun(([](Scene* scene, int id)
		//{
		//	for (auto& object : *scene->getFSEObjects())
		//	{
		//		if (object->getID() == id)
		//			return std::weak_ptr<FSEObject>(object);
		//	}
		//	return std::weak_ptr<FSEObject>();
		//})), "getObjectWithId");
	}

}


template<>
struct v8pp::convert<fse::FSEObject*>
{
	using from_type = fse::FSEObject*;
	using to_type = v8::Local<v8::Object>;

	typedef v8pp::class_<fse::FSEObject> my_class_wrapper;

	static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
	{
		return !value.IsEmpty();
	}

	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		if (!is_valid(isolate, value))
		{
			throw std::invalid_argument("expected FSEObject");
		}

		return my_class_wrapper::unwrap_object(isolate, value);
	}

	static to_type to_v8(v8::Isolate* isolate, fse::FSEObject* const& value)
	{
		auto val = my_class_wrapper::find_object(isolate, value);
		if (val.IsEmpty())
		{
			for (auto& func : fse::fseV8DownCastHelpers<fse::FSEObject>)
			{
				val = func(value, isolate);
				if (!val.IsEmpty())
					return val;
			}
			val = my_class_wrapper::reference_external(isolate, value);
		}
		return val;
	}
};

template<>
struct v8pp::is_wrapped_class<fse::FSEObject*> : std::false_type {};

template<>
struct v8pp::convert<std::shared_ptr<fse::FSEObject>>
{
	using from_type = std::shared_ptr<fse::FSEObject>;
	//using from_type_raw = fse::FSEObject*;
	using to_type = v8::Local<v8::Object>;

	typedef v8pp::class_<fse::FSEObject> my_class_wrapper;

	static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
	{
		return !value.IsEmpty();
	}

	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		if (!is_valid(isolate, value))
		{
			throw std::invalid_argument("expected FSEObject");
		}

		return std::shared_ptr<fse::FSEObject>(my_class_wrapper::unwrap_object(isolate, value));
	}

	static to_type to_v8(v8::Isolate* isolate, std::shared_ptr<fse::FSEObject> const& value)
	{
		const auto raw_ptr = value.get();

		auto val = my_class_wrapper::find_object(isolate, raw_ptr);
		if (val.IsEmpty())
		{
			val = convert<fse::FSEObject*>::to_v8(isolate, raw_ptr);
		}
		return val;
	}
};

template<>
struct v8pp::is_wrapped_class<std::shared_ptr<fse::FSEObject>> : std::false_type {};


template<>
struct v8pp::convert<std::weak_ptr<fse::FSEObject>>
{
	using from_type = std::weak_ptr<fse::FSEObject>;
	//using from_type_raw = fse::FSEObject*;
	using to_type = v8::Local<v8::Object>;

	typedef v8pp::class_<fse::FSEObject> my_class_wrapper;

	static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
	{
		return !value.IsEmpty();
	}

	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		if (!is_valid(isolate, value))
		{
			throw std::invalid_argument("expected FSEObject");
		}

		return std::shared_ptr<fse::FSEObject>(my_class_wrapper::unwrap_object(isolate, value));
	}

	static to_type to_v8(v8::Isolate* isolate, std::weak_ptr<fse::FSEObject> const& value)
	{
		if (value.expired())
		{
			throw std::invalid_argument("Value is expired");
		}
		const auto raw_ptr = value.lock().get();

		auto val = my_class_wrapper::find_object(isolate, raw_ptr);
		if (val.IsEmpty())
		{
			val = convert<fse::FSEObject*>::to_v8(isolate, raw_ptr);

		}
		return val;
	}
};

template<>
struct v8pp::is_wrapped_class<std::weak_ptr<fse::FSEObject>> : std::false_type {};


template<>
struct v8pp::convert<std::vector<std::shared_ptr<fse::FSEObject>>>
{
	using from_type = std::vector<std::shared_ptr<fse::FSEObject>>;
	//using from_type_raw = std::vector<fse::FSEObject*>;
	using to_type = v8::Local<v8::Array>;

	typedef v8pp::class_<fse::FSEObject> my_class_wrapper;

	static bool is_valid(v8::Isolate*, v8::Local<v8::Value> value)
	{
		return !value.IsEmpty() && value->IsArray();
	}

	static from_type from_v8(v8::Isolate* isolate, v8::Local<v8::Value> value)
	{
		if (!is_valid(isolate, value))
		{
			throw std::invalid_argument("expected FSEObject array");
		}

		v8::Local<v8::Array> arr = value.As<v8::Array>();

		from_type result;
		result.reserve(arr->Length());
		for (unsigned i = 0; i < arr->Length(); ++i)
		{
			result.push_back(
				convert<std::shared_ptr<fse::FSEObject>>
				::from_v8(isolate, arr->Get(isolate->GetCurrentContext(), i).ToLocalChecked()));
		}

		return result;
	}

	static to_type to_v8(v8::Isolate* isolate, std::vector<std::shared_ptr<fse::FSEObject>> const& value)
	{
		v8::EscapableHandleScope scope(isolate);

		v8::Local<v8::Array> arr = v8::Array::New(isolate, value.size());
		for (unsigned i = 0; i < arr->Length(); ++i) {
			arr->Set(isolate->GetCurrentContext(), i,
				convert<std::shared_ptr<fse::FSEObject>>
				::to_v8(isolate, value[i]));
		}

		return scope.Escape(arr);
	}
};

template<>
struct v8pp::is_wrapped_class<std::vector<std::shared_ptr<fse::FSEObject>> > : std::false_type {};


