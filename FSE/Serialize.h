#pragma once
#include "FSEObject/FSEObject.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/prettywriter.h"
#include "../rapidjson/fwd.h"

namespace fse
{
	class Serializer
	{
	public:
		Serializer();

		void saveScene(Scene* scene, const std::string& filename);

		void loadScene(Scene* scene, const std::string& filename, bool clear = true);

	private:

		void serializeTypes(rttr::type type, rttr::instance& object,
			rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

		std::unordered_map<rttr::type,
			std::function<void(rttr::variant&, rttr::instance&,
				rapidjson::PrettyWriter<rapidjson::StringBuffer>&)>> serialize_type_funs_;

		void SetupTypes();


		static void FSEObjectFromJson(rapidjson::Value& jsonValue, fse::Scene* scene);

		static void arrayFromJson(rttr::variant& val, rapidjson::Value& jsonValue, fse::Scene* scene);
		static void extractFromJson(rttr::variant& val, rapidjson::Value& jsonValue, fse::Scene* scene);
		static rttr::variant extractBasicTypes(rapidjson::Value& jsonValue);
		static rttr::variant extractObjectFromJson(rapidjson::Value& jsonValue, fse::Scene* scene);

	};

}
