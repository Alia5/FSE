#pragma once

#define NOMINMAX

#include <regex>
#include <rttr/type>
#include <vector>
#include <v8.h>

#include <v8pp/class.hpp>

#define FSE_V8_REGISTRATION_FRIEND friend class fse::priv::FSEV8Lib;

#define FSE_V8_ENABLE(ClassName) \
   static void v8_init_func(v8::Isolate* isolate); \
   static v8_init_helper ClassName##_v8_init_helper

#define FSE_V8_REGISTER(ClassName) \
	v8_init_helper ClassName::ClassName##_v8_init_helper(&ClassName::v8_init_func); \
	void ClassName::v8_init_func(v8::Isolate* isolate)


namespace fse
{
	namespace priv
	{
		class FSEV8Lib
		{
		public:
			static void Init(v8::Isolate* isolate);
		};
	}

	template<typename T>
	void RegisterJSUserTypeFromRTTR(v8::Isolate* isolate)
	{
		std::string typeName = rttr::type::get<T>().get_name().to_string();
		typeName = std::regex_replace(typeName, std::regex("(fse::)"), "");
		typeName = std::regex_replace(typeName, std::regex("(::)"), "");
		////chai.add(chaiscript::user_type<T>(), typeName);
		v8pp::class_<T> bind_T(isolate);
	}

}

typedef void(*init_func_type)(v8::Isolate* isolate);

class v8_init
{
public:
	static v8_init& instance()
	{
		static v8_init inst;
		return inst;
	}

	void add_init_func(init_func_type f) { funcs_.push_back(f); }

	static void execute(v8::Isolate* isolate)
	{
		auto& inst = instance();
		for (auto c : inst.funcs_) c(isolate);
	}

private:
	v8_init() {}

	std::vector<init_func_type> funcs_;
};

class v8_init_helper
{
public:
	v8_init_helper(init_func_type f)
	{
		v8_init::instance().add_init_func(f);
	}
};