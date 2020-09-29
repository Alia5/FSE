#pragma once

#define NOMINMAX

#include <regex>
#include <rttr/type>
#include <vector>
#include <v8.h>

#include <v8pp/module.hpp>
#include <v8pp/class.hpp>

#include "extv8bind/liquidfunconvert.h"
#include "extv8bind/sfmlconvert.h"

#define FSE_V8_REGISTRATION_FRIEND friend class fse::priv::FSEV8Lib;

#define FSE_V8_ENABLE(ClassName) \
   static void v8_init_func(fse::Application* app, v8::Isolate* isolate, v8pp::module& module); \
   static v8_init_helper ClassName##_v8_init_helper

#define FSE_V8_REGISTER(ClassName) \
	v8_init_helper ClassName::ClassName##_v8_init_helper(&ClassName::v8_init_func); \
	void ClassName::v8_init_func(fse::Application* app, v8::Isolate* isolate, v8pp::module& module)

#define FSE_V8_REGISTER_BASE(ClassName) \
	v8_init_helper ClassName::ClassName##_v8_init_helper(&ClassName::v8_init_func, true); \
	void ClassName::v8_init_func(fse::Application* app, v8::Isolate* isolate, v8pp::module& module)

#define FSE_V8_REGISTER_BASE_FIRST(ClassName) \
	v8_init_helper ClassName::ClassName##_v8_init_helper(&ClassName::v8_init_func, true, true); \
	void ClassName::v8_init_func(fse::Application* app, v8::Isolate* isolate, v8pp::module& module)


namespace std {
	namespace filesystem {
		class path;
	}
}

namespace fse
{
	class Application;
	namespace priv
	{
		class FSEV8Require
		{
		public:
			void require(v8::FunctionCallbackInfo<v8::Value> const& args);
			void clearCache();
		private:
			static void v8EvalCatch(v8::TryCatch& try_catch);
			static void consoleError(v8::Isolate* isolate, const std::string& error_string);
			static std::string readFile(const std::filesystem::path& path);
			std::filesystem::path resolveModuleFilePath(const std::filesystem::path& basePath, const std::string& module_name) const;
			void cacheEmptyModule(const std::string& file_path, v8::Isolate* isolate);
			std::map<std::string, v8::Persistent<v8::Value, v8::CopyablePersistentTraits<v8::Value>>> module_cache_;
		};
		
		class FSEV8Lib
		{
		public:
			static void Init(int argc, char* argv[], char** env, fse::Application* app, v8::Isolate* isolate, FSEV8Require* const require);
		private:
			static void loadIndexFile();
		};

		
		template<typename Base, typename Derived>
		v8::Local<v8::Object> downcastHelper(Base * base_ptr, v8::Isolate* isolate)
		{
			if (auto d_ptr = dynamic_cast<Derived*>(base_ptr))
			{
				typedef v8pp::class_<Derived> derived_class_wrapper;
				return static_cast<v8::Local<v8::Object>>(derived_class_wrapper::reference_external(isolate, d_ptr));
			}
			return v8::Local<v8::Object>();
		}

		template<typename Base, typename Derived>
		v8::Local<v8::Object> sharedPtrDowncastHelper(std::shared_ptr<Base> const& base_ptr, v8::Isolate* isolate)
		{
			if (auto d_ptr = std::dynamic_pointer_cast<Derived>(base_ptr))
			{
				typedef v8pp::class_<Derived, v8pp::shared_ptr_traits> derived_class_wrapper;
				return static_cast<v8::Local<v8::Object>>(derived_class_wrapper::reference_external(isolate, d_ptr));
			}
			return v8::Local<v8::Object>();
		}
		
	}

	template<typename Base>
	inline std::vector<std::function<v8::Local<v8::Object>(Base*, v8::Isolate*)>> fseV8DownCastHelpers;
	template<typename Base>
	inline std::vector<std::function<v8::Local<v8::Object>(std::shared_ptr<Base> const&, v8::Isolate*)>> fseV8SharedPtrDownCastHelpers;
	
	template<typename Base, typename Derived>
	void addV8DownCastHelper()
	{
		fseV8DownCastHelpers<Base>.push_back(priv::downcastHelper<Base, Derived>);
		fseV8SharedPtrDownCastHelpers<Base>.push_back(priv::sharedPtrDowncastHelper<Base, Derived>);
	}

	
	template<typename T>
	v8pp::class_<T> RegisterJSUserTypeFromRTTR(v8::Isolate* isolate)
	{
		std::string typeName = rttr::type::get<T>().get_name().to_string();
		typeName = std::regex_replace(typeName, std::regex("(fse::)"), "");
		typeName = std::regex_replace(typeName, std::regex("(::)"), "");
		v8pp::class_<T> bind_T(isolate);
		return bind_T;
	}

}

typedef void(*init_func_type)(fse::Application* app, v8::Isolate* isolate, v8pp::module& module);

class v8_init
{
public:
	static v8_init& instance()
	{
		static v8_init inst;
		return inst;
	}

	void add_init_func(init_func_type f, bool base = false, bool first = false)
	{
		if (base)
		{
			if (first && !base_funcs_.empty())
			{
				base_funcs_.insert(base_funcs_.begin(), f);
			} else {
				base_funcs_.push_back(f);
			}
			return;
		}
		funcs_.push_back(f);
	}

	static void execute(fse::Application* app, v8::Isolate* isolate, v8pp::module& module)
	{
		auto& inst = instance();
		for (auto b : inst.base_funcs_) b(app, isolate, module);
		for (auto c : inst.funcs_) c(app, isolate, module);
	}


private:
	v8_init() {}

	std::vector<init_func_type> funcs_;
	std::vector<init_func_type> base_funcs_;
};

class v8_init_helper
{
public:
	v8_init_helper(init_func_type f, bool base = false, bool first = false)
	{
		v8_init::instance().add_init_func(f, base, first);
	}
	
};