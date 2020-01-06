#include "FSEV8Lib.h"
#include "FSEObject/KillVolume.h"
#include "Lights/FSELightWorld.h"
#include "FSEObject/FPSCounter.h"
#include "FSEObject/Timer.h"
#include "Lights/Light.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "Random.h"
#include "Scene.h"
#include <functional>
#include <filesystem>
#include <string>
#include <sstream>
#include "Application.h"

#include "extv8bind/liquidfunbind.h"
#include "extv8bind/sfmlbind.h"


namespace fse
{
	namespace priv
	{
		void require(v8::FunctionCallbackInfo<v8::Value> const&);
		void FSEV8Lib::Init(int argc, char* argv[], char** env, fse::Application* app, v8::Isolate* isolate, FSEV8Require* const require)
		{

				v8::HandleScope handle_scope(isolate);
				const auto ctx = isolate->GetCurrentContext();
				ctx->Global()->Set(ctx,
					v8::String::NewFromUtf8(isolate, "global").ToLocalChecked(),
					ctx->Global());
				ctx->Global()->Set(ctx,
					v8::String::NewFromUtf8(isolate, "GLOBAL").ToLocalChecked(),
					ctx->Global());

				if (argc > 0)
				{
					v8pp::module processModule(isolate);
					const auto procMod = processModule.new_instance();
					v8::Local<v8::Array> v8argv = v8::Array::New(isolate, argc);
					for (auto i = 0u; i < argc; i++)
					{
						v8argv->Set(ctx, i, v8pp::to_v8(isolate, argv[i]));
					}
					procMod->Set(ctx, v8pp::to_v8(isolate, "argv"), v8argv);
					if (env != nullptr)
					{
						v8::Local<v8::Object> envObj = v8::Object::New(isolate);
						for (auto i = 0u; env[i] != nullptr; i++)
						{
							envObj->Set(ctx,
								v8pp::to_v8(isolate, std::regex_replace(env[i], std::regex("=.*"), "")),
									v8pp::to_v8(isolate, std::regex_replace(env[i], std::regex(".*?="), "")));
						}
						procMod->Set(ctx, v8pp::to_v8(isolate, "env"), v8pp::to_v8(isolate, envObj));
					}
					ctx->Global()->Set(ctx,
						v8::String::NewFromUtf8(isolate, "process").ToLocalChecked(), procMod);
					
				}
			
				v8pp::module module(isolate);
			
				////chai.add(chaiscript::extras::math::bootstrap());
				////chai.add(chaiscript::extras::string_methods::bootstrap());

				////chai.add(chaiscript::extras::box2d::bootstrap());

				////chai.add(chaiscript::fun([](const b2Body* body)
				//{
				//	return static_cast<FSEObject*>(body->GetUserData());
				//}), "GetUserData");
				////chai.add(chaiscript::fun([](b2Body* body, FSEObject* data)
				//{
				//	body->SetUserData(data);
				//}), "SetUserData");

				////chai.add(chaiscript::fun([](const b2Fixture* fixture)
				//{
				//	return static_cast<FSEObject*>(fixture->GetUserData());
				//}), "GetUserData");
				////chai.add(chaiscript::fun([](b2Fixture* fixture, FSEObject* data)
				//{
				//	fixture->SetUserData(data);
				//}), "SetUserData");


				////chai.add(chaiscript::fun([](const b2ParticleDef* pdef)
				//{
				//	return static_cast<FSEObject*>(pdef->userData);
				//}), "GetUserData");
				////chai.add(chaiscript::fun([](b2ParticleDef* pdef, FSEObject* data)
				//{
				//	pdef->userData = data;
				//}), "SetUserData");


				////chai.add(chaiscript::fun([](const b2ParticleGroupDef* pdef)
				//{
				//	return static_cast<FSEObject*>(pdef->userData);
				//}), "GetUserData");
				////chai.add(chaiscript::fun([](b2ParticleGroupDef* pdef, FSEObject* data)
				//{
				//	pdef->userData = data;
				//}), "SetUserData");

				////chai.add(chaiscript::fun([](const b2ParticleGroup* group)
				//{
				//	return static_cast<FSEObject*>(group->GetUserData());
				//}), "GetUserData");
				////chai.add(chaiscript::fun([](b2ParticleGroup* group, FSEObject* data)
				//{
				//	group->SetUserData(data);
				//}), "SetUserData");

				////chai.add(chaiscript::fun([](const b2ParticleSystem* system)
				//{
				//	return (FSEObject**)(system->GetUserDataBuffer());
				//}), "GetUserData");
				////chai.add(chaiscript::fun([](const b2ParticleSystem* system)
				//{
				//	return reinterpret_cast<FSEObject* const*>(system->GetUserDataBuffer());
				//}), "GetUserData");
				////chai.add(chaiscript::fun([](b2ParticleSystem* system, FSEObject** data, const int capacity)
				//{
				//	system->SetUserDataBuffer(reinterpret_cast<void**>(data), capacity);
				//}), "SetUserData");


				////chai.add(fse::Random::bootstrap());

				////chai.add(chaiscript::extras::sfml::bootstrap());

				////chai.add(chaiscript::type_conversion<sf::Vector2f, b2Vec2>([](const sf::Vector2f& vec) { return b2Vec2(vec.x, vec.y); }));
				////chai.add(chaiscript::type_conversion<b2Vec2, sf::Vector2f>([](const b2Vec2& vec) { return sf::Vector2f(vec.x, vec.y); }));

				////chai.add(chaiscript::fun([](sf::Vector2f& lhs, const b2Vec2& rhs) { return lhs = sf::Vector2f(rhs.x, rhs.y); }), "=");
				////chai.add(chaiscript::fun([](b2Vec2& lhs, const sf::Vector2f& rhs) { return lhs = b2Vec2(rhs.x, rhs.y); }), "=");

				////chai.add(chaiscript::fun([](const sf::Vector2f& lhs, const b2Vec2& rhs) { return lhs == sf::Vector2f(rhs.x, rhs.y); }), "==");
				////chai.add(chaiscript::fun([](const b2Vec2& lhs, const sf::Vector2f& rhs) { return lhs == b2Vec2(rhs.x, rhs.y); }), "==");


				////chai.add(chaiscript::vector_conversion<std::vector<std::shared_ptr<FSEObject>>>());
				////chai.add(chaiscript::bootstrap::standard_library::vector_type<std::vector<std::shared_ptr<FSEObject>>>("ObjectList"));


				////chai.add(chaiscript::vector_conversion<std::vector<std::weak_ptr<FSEObject>>>());
				////chai.add(chaiscript::bootstrap::standard_library::vector_type<std::vector<std::weak_ptr<FSEObject>>>("WeakObjectList"));

				////chai.add(chaiscript::vector_conversion<std::vector<Light*>>());
				////chai.add(chaiscript::bootstrap::standard_library::vector_type<std::vector<Light*>>("LightList"));

				v8_init::execute(app, isolate, module);
				ctx->Global()->Set(ctx,
					v8::String::NewFromUtf8(isolate, "fse").ToLocalChecked(), module.new_instance());

				typedef v8pp::class_<fse::Scene> my_class_wrapper;
				v8::Local<v8::Value> val = my_class_wrapper::reference_external(isolate, &app->getRootScene());
				ctx->Global()->Set(ctx, v8::String::NewFromUtf8(isolate, "rootScene").ToLocalChecked(), val);
			
				v8pp::module require_mod(isolate);
				v8pp::class_<FSEV8Require> require_class(isolate);
				require_class.function("require", &FSEV8Require::require);
				require_mod.class_("FSEV8Require", require_class);
				require_mod.function("__v8require", [require](v8::FunctionCallbackInfo<v8::Value> const& args)
					{
						return require->require(args);
					});
			
				v8::Local<v8::Object> req_mod = require_mod.new_instance();
				ctx->Global()->Set(ctx,
					v8::String::NewFromUtf8(isolate, "V8Require").ToLocalChecked(), req_mod);
				ctx->Global()->Set(ctx,
					v8::String::NewFromUtf8(isolate, "require").ToLocalChecked(),
					req_mod->Get(ctx,
						v8::String::NewFromUtf8(isolate, "__v8require").ToLocalChecked()).ToLocalChecked());


				const v8::Local<v8::Object> b2Mod = getB2Mod(isolate).new_instance();
				ctx->Global()->Set(ctx,
					v8::String::NewFromUtf8(isolate, "lf").ToLocalChecked(), b2Mod);
				const v8::Local<v8::Object> sfMod = getSFMod(isolate).new_instance();
				ctx->Global()->Set(ctx,
					v8::String::NewFromUtf8(isolate, "sf").ToLocalChecked(), sfMod);
			
		}

		std::filesystem::path FSEV8Require::resolveModuleFilePath(const std::filesystem::path& basePath, const std::string& module_name) const
		{
			std::filesystem::path workingPath = std::filesystem::path(basePath).append(module_name);
			if (!basePath.has_parent_path() && !std::filesystem::exists(basePath))
			{
				return std::filesystem::path();
			}
			if (std::filesystem::exists(workingPath)
					&& (std::filesystem::is_regular_file(workingPath) || std::filesystem::is_symlink(workingPath)))
			{
				return std::filesystem::absolute(workingPath);
			}
			workingPath = std::filesystem::path(basePath).append(module_name + ".js");
			if (std::filesystem::exists(workingPath)
				&& (std::filesystem::is_regular_file(workingPath) || std::filesystem::is_symlink(workingPath)))
			{
				return std::filesystem::absolute(workingPath);
			}
			workingPath = std::filesystem::path(basePath).append(module_name).append("index.js");
			if (std::filesystem::exists(workingPath)
				&& (std::filesystem::is_regular_file(workingPath) || std::filesystem::is_symlink(workingPath)))
			{
				return std::filesystem::absolute(workingPath);
			}
			workingPath = std::filesystem::path(basePath).append("node_modules").append(module_name).append("index.js");
			if (std::filesystem::exists(workingPath)
				&& (std::filesystem::is_regular_file(workingPath) || std::filesystem::is_symlink(workingPath)))
			{
				return std::filesystem::absolute(workingPath);
			}
			workingPath = std::filesystem::path(basePath).append("node_modules").append(module_name).append(module_name + ".js");
			if (std::filesystem::exists(workingPath)
				&& (std::filesystem::is_regular_file(workingPath) || std::filesystem::is_symlink(workingPath)))
			{
				return std::filesystem::absolute(workingPath);
			}
			workingPath = std::filesystem::current_path().append("js").append(module_name);
			if (std::filesystem::exists(workingPath)
				&& (std::filesystem::is_regular_file(workingPath) || std::filesystem::is_symlink(workingPath)))
			{
				return std::filesystem::absolute(workingPath);
			}
			workingPath = std::filesystem::current_path().append("js").append(module_name + ".js");
			if (std::filesystem::exists(workingPath)
				&& (std::filesystem::is_regular_file(workingPath) || std::filesystem::is_symlink(workingPath)))
			{
				return std::filesystem::absolute(workingPath);
			}
			workingPath = std::filesystem::current_path().append("js").append("node_modules").append(module_name).append("index.js");
			if (std::filesystem::exists(workingPath)
				&& (std::filesystem::is_regular_file(workingPath) || std::filesystem::is_symlink(workingPath)))
			{
				return std::filesystem::absolute(workingPath);
			}
			workingPath = std::filesystem::current_path().append("js").append("node_modules").append(module_name).append( module_name + ".js");
			if (std::filesystem::exists(workingPath)
				&& (std::filesystem::is_regular_file(workingPath) || std::filesystem::is_symlink(workingPath)))
			{
				return std::filesystem::absolute(workingPath);
			}
			return std::filesystem::path();
		}

		void FSEV8Require::cacheEmptyModule(const std::string& file_path, v8::Isolate* isolate)
		{
			module_cache_[file_path].Reset(isolate, v8pp::module(isolate).new_instance());
		}

		std::string FSEV8Require::readFile(const std::filesystem::path& path)
		{
			std::ifstream inFile;
			std::stringstream sb;
			inFile.open(path.string());
			if (!inFile) {
				throw std::exception("No such file");
			}
			sb << inFile.rdbuf();
			inFile.close();
			return sb.str();
		}

		void FSEV8Require::consoleError(v8::Isolate* isolate, const std::string& error_string)
		{
			const v8::Local<v8::Context> ctx = isolate->GetEnteredContext();
			const v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, std::string("console.error('" + error_string + "');").data(),
				v8::NewStringType::kNormal).ToLocalChecked();
			v8::MaybeLocal<v8::Script> maybe_script = v8::Script::Compile(ctx, source);
			if (maybe_script.IsEmpty())
				return;
			v8::Local<v8::Script> script = maybe_script.ToLocalChecked();
			script->Run(ctx);
		}
		
		void FSEV8Require::v8EvalCatch(v8::TryCatch& try_catch)
		{
			v8::Isolate* iso = v8::Isolate::GetCurrent();
			if (try_catch.HasCaught())
			{
				const std::string errorString(*v8::String::Utf8Value(iso, try_catch.Message()->Get()));
				if (!try_catch.CanContinue())
				{
					throw std::exception(errorString.c_str());
				}
				consoleError(iso, errorString);
			}
			try_catch.Reset();
		}
		
		void FSEV8Require::require(v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			v8::Isolate* isolate = args.GetIsolate();
			if (args.Length() != 1 || !args[0]->IsString())
			{
				isolate->ThrowException(v8::String::NewFromUtf8(isolate, "expected string").ToLocalChecked());
				args.GetReturnValue().Set(Undefined(args.GetIsolate()));
				return;
			}
			v8::String::Utf8Value required_module(isolate, args[0]);

			// TODO: Check if system module (i.e. fs, net, ...)
			// TODO: Check if core module (i.e. entity, renderer, resource, etc.)

			// Get current script filepath
			v8::String::Utf8Value
				current_script_filepath_v8(isolate,
					v8::StackTrace::CurrentStackTrace(isolate, 1, v8::StackTrace::kScriptName)
					->GetFrame(isolate, 0)->GetScriptName());

			std::string current_script_filepath = std::filesystem::current_path().string();
			if (current_script_filepath_v8.length() > 0 && std::filesystem::exists(*current_script_filepath_v8))
			{
				current_script_filepath = std::filesystem::path(*current_script_filepath_v8).parent_path().string();
			}
			
			const std::filesystem::path current_script_path(current_script_filepath);
			auto script_file_path = resolveModuleFilePath(current_script_filepath, *required_module);
		
			if (script_file_path.empty())
			{
				args.GetReturnValue().Set(Undefined(args.GetIsolate()));
			}

			// Is module already in cache?
			const auto cached_module_itr = module_cache_.find(script_file_path.string());
			if (cached_module_itr != module_cache_.end())
			{
				std::cout << "Loaded module " << script_file_path.string() << " from cache\n";
				args.GetReturnValue().Set(cached_module_itr->second.Get(isolate));
				//v8::StrongPersistentToLocal(cached_module_itr->second)
				return;
			}

			std::string fileContent;
			try
			{
				fileContent = readFile(script_file_path);
			} catch (const std::exception& e) {

				isolate->ThrowException(v8::String::NewFromUtf8(isolate, e.what()).ToLocalChecked());
				args.GetReturnValue().Set(Undefined(args.GetIsolate()));
				return;
			}
			if (fileContent.length() > 0)
			{
				std::stringstream sb;
				sb << "\
					  (function (__scriptFilename, __scriptDirname) {\n\
						  var module = {\n\
							exports: {}\n\
						  }, exports = module.exports;\n\
						const __filename = __scriptFilename, __dirname = __scriptDirname;\n"
						<< fileContent
						<< "\n; return module.exports;\n\
						}('" << std::regex_replace(script_file_path.string(), std::regex("\\\\"), "\\\\")
								<< "', '" << std::regex_replace(script_file_path.parent_path().string(), std::regex("\\\\"), "\\\\") << "'));";

				const v8::Local<v8::Context> ctx = isolate->GetEnteredContext();
				v8::TryCatch try_catch(isolate);
				try_catch.SetCaptureMessage(true);
				try_catch.SetVerbose(true);

				v8::MaybeLocal<v8::String> maybe_source =
					v8::String::NewFromUtf8(isolate, sb.str().data(),
						v8::NewStringType::kNormal);
				
				if (maybe_source.IsEmpty())
				{
					v8EvalCatch(try_catch);
					return;
				}
				const v8::Local<v8::String> source = maybe_source.ToLocalChecked();
				v8::ScriptOrigin origin(v8pp::to_v8(isolate, script_file_path.string()),
					v8::Integer::New(isolate, -5),
					v8::Integer::New(isolate, 0),             
					v8::False(isolate),                      
					v8::Local<v8::Integer>(),                     
					v8::Local<v8::Value>(),                       
					v8::False(isolate),                      
					v8::False(isolate),                      
					v8::False(isolate));                      
				v8::MaybeLocal<v8::Script> maybe_script = v8::Script::Compile(ctx, source, &origin);
				if (maybe_script.IsEmpty())
				{
					v8EvalCatch(try_catch);
					return;
				}
				v8::Local<v8::Script> script = maybe_script.ToLocalChecked();

				
				cacheEmptyModule(script_file_path.string(), isolate);
				v8::MaybeLocal<v8::Value> maybe_result = script->Run(ctx);
				if (maybe_result.IsEmpty())
				{
					v8EvalCatch(try_catch);
					return;
				}
				const v8::Local<v8::Value> exports = maybe_result.ToLocalChecked();

				module_cache_[script_file_path.string()].Reset(args.GetIsolate(), exports);

				args.GetReturnValue().Set(exports);
			}
			else
			{
				args.GetReturnValue().Set(Undefined(args.GetIsolate()));
			}
		}

		void FSEV8Require::clearCache()
		{
			module_cache_.clear();
		}
	}
}
