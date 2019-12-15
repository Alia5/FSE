#pragma once

#define NOMINMAX

#include <regex>
#include <rttr/type>
#include <vector>
#include <v8.h>

#include <v8pp/module.hpp>
#include <v8pp/class.hpp>

#define FSE_V8_REGISTRATION_FRIEND friend class fse::priv::FSEV8Lib;

#define FSE_V8_ENABLE(ClassName) \
   static void v8_init_func(v8::Isolate* isolate, v8pp::module& module); \
   static v8_init_helper ClassName##_v8_init_helper

#define FSE_V8_REGISTER(ClassName) \
	v8_init_helper ClassName::ClassName##_v8_init_helper(&ClassName::v8_init_func); \
	void ClassName::v8_init_func(v8::Isolate* isolate, v8pp::module& module)

#define FSE_V8_REGISTER_BASE(ClassName) \
	v8_init_helper ClassName::ClassName##_v8_init_helper(&ClassName::v8_init_func, true); \
	void ClassName::v8_init_func(v8::Isolate* isolate, v8pp::module& module)


namespace fse
{
	namespace priv
	{
		class FSEV8Lib
		{
		public:
			static void Init(v8::Isolate* isolate);
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
		
	}

	template<typename Base>
	inline std::vector<std::function<v8::Local<v8::Object>(Base*, v8::Isolate*)>> fseV8DownCastHelpers;
	
	template<typename Base, typename Derived>
	void addV8DownCastHelper()
	{
		fseV8DownCastHelpers<Base>.push_back(priv::downcastHelper<Base, Derived>);
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

typedef void(*init_func_type)(v8::Isolate* isolate, v8pp::module& module);

class v8_init
{
public:
	static v8_init& instance()
	{
		static v8_init inst;
		return inst;
	}

	void add_init_func(init_func_type f, bool base = false)
	{
		if (base)
		{
			base_funcs_.push_back(f);
			return;
		}
		funcs_.push_back(f);
	}

	static void execute(v8::Isolate* isolate, v8pp::module& module)
	{
		auto& inst = instance();
		for (auto b : inst.base_funcs_) b(isolate, module);
		for (auto c : inst.funcs_) c(isolate, module);
	}


private:
	v8_init() {}

	std::vector<init_func_type> funcs_;
	std::vector<init_func_type> base_funcs_;
};

class v8_init_helper
{
public:
	v8_init_helper(init_func_type f, bool base = false)
	{
		v8_init::instance().add_init_func(f, base);
	}
	
};