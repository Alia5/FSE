#pragma once

#define NOMINMAX

#include <chaiscript/chaiscript.hpp>
#include <regex>
#include <rttr/type>


namespace fse
{
	namespace priv
	{
		class FSEChaiLib
		{
		public:
			static void Init(chaiscript::ChaiScript& chai);

		private:
			template<typename T>
			static void RegisterChaiUserTypeFromRTTR(chaiscript::ChaiScript& chai)
			{
				std::string typeName = rttr::type::get<T>().get_name().to_string();
				typeName = std::regex_replace(typeName, std::regex("(fse::)"), "");
				typeName = std::regex_replace(typeName, std::regex("(::)"), "");
				chai.add(chaiscript::user_type<T>(), typeName);
			}

			static void RegisterFSEObjects(chaiscript::ChaiScript& chai);

			static void RegisterObjectCtors(chaiscript::ChaiScript& chai);

		};
	}
}

