#pragma once

#define NOMINMAX

#include <chaiscript/chaiscript.hpp>
#include <regex>
#include <rttr/type>
#include <vector>



#define FSE_CHAI_REGISTRATION_FRIEND friend class fse::priv::FSEChaiLib;

#define FSE_CHAI_ENABLE(ClassName) \
   static void chai_init_func(chaiscript::ChaiScript& chai); \
   static chai_init_helper ClassName##_chai_init_helper

#define FSE_CHAI_REGISTER(ClassName) \
	chai_init_helper ClassName::ClassName##_chai_init_helper(&ClassName::chai_init_func); \
	void ClassName::chai_init_func(chaiscript::ChaiScript& chai)


namespace fse
{
	namespace priv
	{
		class FSEChaiLib
		{
		public:
			static void Init(chaiscript::ChaiScript& chai);
		};
	}

	template<typename T>
	void RegisterChaiUserTypeFromRTTR(chaiscript::ChaiScript& chai)
	{
		std::string typeName = rttr::type::get<T>().get_name().to_string();
		typeName = std::regex_replace(typeName, std::regex("(fse::)"), "");
		typeName = std::regex_replace(typeName, std::regex("(::)"), "");
		chai.add(chaiscript::user_type<T>(), typeName);
	}

}

typedef void(*init_func_type)(chaiscript::ChaiScript&);

class chai_init
{
public:
	static chai_init& instance()
	{
		static chai_init inst;
		return inst;
	}

	void add_init_func(init_func_type f) { funcs_.push_back(f); }

	static void execute(chaiscript::ChaiScript& chai)
	{
		auto& inst = instance();
		for (auto c : inst.funcs_) c(chai);
	}

private:
	chai_init() {}

	std::vector<init_func_type> funcs_;
};

class chai_init_helper
{
public:
	chai_init_helper(init_func_type f)
	{
		chai_init::instance().add_init_func(f);
	}
};