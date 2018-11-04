#include "Serialize.h"

#include <rapidjson/prettywriter.h>
#include <rapidjson/document.h>

#include <fstream>
#include <iostream>

#include "Application.h"

namespace fse
{
	Serializer::Serializer()
	{
		SetupTypes();
	}


	void Serializer::saveScene(Scene* scene, const std::string& filename)
	{

		auto objects = scene->getFSEObjects();

		rapidjson::StringBuffer sb;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

		writer.SetMaxDecimalPlaces(8);
		writer.StartObject();

		writer.Key("Objects");
		writer.StartArray();

		for (const auto& object : *objects)
		{
			auto type = rttr::type::get(*object);
			auto inst = rttr::instance(*object);

			if (type.get_metadata("NO_SERIALIZE"))
				continue;

			serializeTypes(type, inst, writer);
		}

		writer.EndArray();

		writer.EndObject();

		std::string buff(sb.GetString());

		std::ofstream stream;

		stream.open(filename, std::ios::out);

		if (stream.is_open())
		{
			stream << buff;

			stream.close();
		} else
		{
			std::cout << "error writing file\n";
		}

		//std::cout << "============================\n";
		//std::cout << buff << "\n";
		//std::cout << "============================\n";
	}

	void Serializer::loadScene(Scene* scene, const std::string& filename, bool clear)
	{

		rapidjson::Document document;  // Default template parameter uses UTF8 and MemoryPoolAllocator.

		std::string json;
		
		std::ifstream ifstream;

		ifstream.open(filename, std::ios::in);

		if (ifstream.is_open())
		{
			
			json = std::string(std::istreambuf_iterator<char>(ifstream), std::istreambuf_iterator<char>());

			ifstream.close();
		} else {
			std::cout << "error reading file\n";
			return;
		}

		if (document.Parse(json.c_str()).HasParseError())
		{
			std::cout << "document malformed\n";
			return;
		}

		rapidjson::Value::MemberIterator member = document.FindMember("Objects");

		auto& jsonVal = member->value;

		if (jsonVal.GetType() != rapidjson::kArrayType)
		{
			std::cout << "document malformed\n";
			return;
		} 

		if (clear)
		{
			auto objs = scene->getFSEObjects();
			for (auto& obj : *objs)
			{
				static_cast<FSEObject*>(obj.get())->destroy();
			}
		}


		rttr::variant var;
		auto array_view = var.create_sequential_view();

		array_view.set_size(jsonVal.Size());

		for (size_t i = 0; i < jsonVal.Size(); i++)
		{
			auto& json_index_value = jsonVal[i];
			if (json_index_value.GetType() == rapidjson::kObjectType)
			{
				rttr::variant val;
				extractFromJson(val, json_index_value, scene);
			}
		}
		scene->getApplication()->initChai();

	}

	void Serializer::serializeTypes(rttr::type type, rttr::instance& object, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
	{
		if (type.is_pointer())
		{
			auto t = object.get_derived_type();
			serializeTypes(t, object, writer);
			return;
		}

		writer.StartObject();

		writer.Key("type");
		std::string stri(type.get_name().data());
		writer.String(stri.data(), stri.size());

		for (auto& prop : type.get_properties())
		{
			if (prop.get_metadata("NO_SERIALIZE"))
				continue;

			if (prop.is_readonly() && !prop.get_metadata("CTOR_ARG"))
				continue;

			auto prop_value = prop.get_value(object);
			if (!prop_value)
				continue; // cannot serialize, because we cannot retrieve the value


			if (serialize_type_funs_.count(prop.get_type()))
			{
				auto val = prop.get_value(object);			
				writer.Key(prop.get_name().data(), prop.get_name().size());
				serialize_type_funs_[prop.get_type()](val, object, writer);
			}

			else if (prop.get_type().get_properties().size() > 0)
			{
				auto val = prop.get_value(object);
				auto inst = rttr::instance(val);
				writer.Key(prop.get_name().data(), prop.get_name().size());
				serializeTypes(prop.get_type(), inst, writer);
			}

			else if (prop.get_type().is_array())
			{
				auto val = prop.get_value(object);
				rttr::variant_sequential_view arr = val.create_sequential_view();
				size_t sz = arr.get_size();

				writer.Key(prop.get_name().data(), prop.get_name().size());
				writer.StartArray();

				for (size_t i = 0; i < sz; i++)
				{
					auto v = arr.get_value(i);
					auto t = v.get_type();
					auto inst = rttr::instance(v);
					serializeTypes(t, inst, writer);
				}

				writer.EndArray();
			} 



		}


		writer.EndObject();
	}


	void Serializer::SetupTypes()
	{
		serialize_type_funs_[rttr::type::get<bool>()] =
			[](rttr::variant& variant, rttr::instance& instance, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
		{
			writer.Bool(variant.convert<bool>());
		};

		serialize_type_funs_[rttr::type::get<int>()] = 
			[](rttr::variant& variant, rttr::instance& instance, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
		{
			writer.Int(variant.convert<int>());
		};

		serialize_type_funs_[rttr::type::get<float>()] =
			[](rttr::variant& variant, rttr::instance& instance, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
		{
			writer.Double(variant.convert<float>());
		};

		serialize_type_funs_[rttr::type::get<std::string>()] =
			[](rttr::variant& variant, rttr::instance& instance, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
		{
			writer.String(variant.convert<std::string>().data());
		};

		serialize_type_funs_[rttr::type::get<sf::Vector2f>()] = 
			[](rttr::variant& variant, rttr::instance& instance, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
		{
			writer.StartObject();

			writer.Key("type");
			std::string typestr(variant.get_type().get_name().data());
			writer.String(typestr.data(), typestr.size());

			sf::Vector2f vec = variant.convert<sf::Vector2f>();
			writer.String("x");
			writer.Double(vec.x);
			writer.String("y");
			writer.Double(vec.y);

			writer.EndObject();
		};

		serialize_type_funs_[rttr::type::get<sf::Color>()] = 
			[](rttr::variant& variant, rttr::instance& instance, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
		{
			writer.StartObject();

			writer.Key("type");
			std::string typestr(variant.get_type().get_name().data());
			writer.String(typestr.data(), typestr.size());

			sf::Color col = variant.convert<sf::Color>();
			writer.String("r");
			writer.Int(col.r);
			writer.String("g");
			writer.Int(col.g);
			writer.String("b");
			writer.Int(col.b);
			writer.String("a");
			writer.Int(col.a);

			writer.EndObject();
		};

		serialize_type_funs_[rttr::type::get<sf::IntRect>()] =
			[](rttr::variant& variant, rttr::instance& instance, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
		{
			writer.StartObject();

			writer.Key("type");
			std::string typestr(variant.get_type().get_name().data());
			writer.String(typestr.data(), typestr.size());

			sf::IntRect rekt = variant.convert<sf::IntRect>();
			writer.String("left");
			writer.Int(rekt.left);
			writer.String("top");
			writer.Int(rekt.top);
			writer.String("width");
			writer.Int(rekt.width);
			writer.String("height");
			writer.Int(rekt.height);

			writer.EndObject();
		};

		serialize_type_funs_[rttr::type::get<sf::FloatRect>()] =
			[](rttr::variant& variant, rttr::instance& instance, rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
		{
			writer.StartObject();

			writer.Key("type");
			std::string typestr(variant.get_type().get_name().data());
			writer.String(typestr.data(), typestr.size());

			sf::FloatRect rekt = variant.convert<sf::FloatRect>();
			writer.String("left");
			writer.Double(rekt.left);
			writer.String("top");
			writer.Double(rekt.top);
			writer.String("width");
			writer.Double(rekt.width);
			writer.String("height");
			writer.Double(rekt.height);

			writer.EndObject();
		};

	}

	void Serializer::FSEObjectFromJson(rapidjson::Value& jsonValue, fse::Scene* scene)
	{
		auto tmemb = jsonValue.FindMember("type");
		if (tmemb == jsonValue.MemberEnd())
			return;

		auto& typeval = tmemb->value;
		if (typeval.GetType() != rapidjson::kStringType)
			return;

		auto type = rttr::type::get_by_name(typeval.GetString());

		if (type.get_metadata("SERIALIZE_NO_RECRATE"))
		{		
			FSEObject* ptr = nullptr;

			auto objects = scene->getFSEObjects();
			for (auto& obj : *objects)
			{
				if (rttr::type::get(*obj.get()) == type)
				{
					ptr = obj.get();
				}
			}

			if (ptr == nullptr)
				return;

			auto inst = rttr::instance(ptr);

			for (auto& prop : type.get_properties())
			{
				if (prop.is_readonly())
					continue;

				std::string key(prop.get_name().data());
				auto json_memb = jsonValue.FindMember(key.data());
				if (json_memb == jsonValue.MemberEnd())
					continue;

				auto& tval = json_memb->value;
				auto val = prop.get_value(inst);
				extractFromJson(val, tval, scene);
				if (val.get_type() != prop.get_type())
				{
					if (!val.convert(prop.get_type()))
						continue;
				}

				if (val != prop.get_value(inst))
					prop.set_value(inst, val);
			}

			return;
		}

		auto ctors = type.get_constructors();

		if (ctors.size() == 0)
		{
			std::cout << "FSEObject without constructor!!!\n";
			return;
		}

		std::vector<std::string> ctor_args;
		auto props = type.get_properties();
		for (auto& prop : props)
		{
			auto meta = prop.get_metadata("CTOR_ARG");
			if (meta)
				ctor_args.push_back(meta.to_string());
		}

		auto ctor = *ctors.begin();
		// Search suitable ctor with most args //
		for (auto ctor_it = ctors.rbegin(); ctor_it != ctors.rend(); ++ctor_it)
		{
			auto infos = ctor_it->get_parameter_infos();

			bool ok = true;
			if (infos.size() > 2)
			{
				for (auto it = infos.rbegin(); it != infos.rend(); ++it)
				{
					if (it->get_index() < 2)
						break;

					std::string paramName(it->get_name().data());
					if (std::find(ctor_args.begin(), ctor_args.end(), paramName) == ctor_args.end())
					{
						ok = false;
					}
				}
			}

			if (ok)
			{
				ctor = *ctor_it;
				std::vector<rttr::variant> spawn_args;

				std::vector<std::string> used_json_vals;
				used_json_vals.push_back("type");

				for (auto& info : infos)
				{
					if (info.get_index() == 0)
					{
						auto val = rttr::variant(scene);
						spawn_args.push_back(val);
						continue;
					}

					if (info.get_index() == 1)
					{
						auto& tval = jsonValue.FindMember("position_")->value;
						rttr::variant var(sf::Vector2f(0.f, 0.f));
						extractObjectFromJson(var, tval, scene);
						spawn_args.push_back(var);
						used_json_vals.push_back("position_");

						continue;
					}

					for (auto& prop : props)
					{
						if (prop.get_metadata("CTOR_ARG").to_string() == info.get_name().to_string())
						{
							std::string key(prop.get_name().data());

							auto json_memb = jsonValue.FindMember(key.data());

							if (json_memb == jsonValue.MemberEnd())
								break;

							auto& tval = json_memb->value;

							rttr::variant val;
							val.convert(prop.get_type());
							extractFromJson(val, tval, scene);

							info.get_type();
							if (val.get_type() != info.get_type())
							{
								val.convert(info.get_type());
							}

							spawn_args.push_back(val);

							used_json_vals.push_back(key);
						}
					}
				}

				size_t sz = scene->pending_object_spawns_.size();

				//somehow has to be this way, if pushed back directly in an argument vector some values are wrong... o.O
				std::vector<rttr::argument> arguvec;
				for (auto & arg : spawn_args)
					arguvec.push_back(arg);
				if (arguvec.size() > 0)
					ctor_it->invoke_variadic(arguvec);

				if (scene->pending_object_spawns_.size() == (sz + 1))
				{
					auto it = scene->pending_object_spawns_.rbegin();
					FSEObject* ptr = (*it).get();

					auto t = rttr::type::get(*ptr);
					if (t != type)
						break;

					for (auto& prop : type.get_properties())
					{
						if (prop.is_readonly())
							continue;
						if (std::find(used_json_vals.begin(), used_json_vals.end(), std::string(prop.get_name().data()))
							!= used_json_vals.end())
							continue;

						std::string key(prop.get_name().data());
						auto json_memb = jsonValue.FindMember(key.data());
						if (json_memb == jsonValue.MemberEnd())
							continue;

						auto& tval = json_memb->value;
						auto val = prop.get_value(ptr);
						extractFromJson(val, tval, scene);
						if (val.get_type() != prop.get_type())
						{
							val.convert(prop.get_type());
						}

						if (val != prop.get_value(ptr))
							prop.set_value(ptr, val);
					}
				}
				break;
			}
		}
	}

	void Serializer::arrayFromJson(rttr::variant& val, rapidjson::Value& jsonValue, fse::Scene* scene)
	{
		auto array_view = val.create_sequential_view();
		auto size = jsonValue.Size();
		auto sizeBf = array_view.get_size();
		array_view.set_size(size);

		for (size_t i = 0; i < size; i++)
		{
			auto& json_index_value = jsonValue[i];
			rttr::variant var;
			if (i < sizeBf)
				var = array_view.get_value(i);
			extractFromJson(var, json_index_value, scene);
			array_view.set_value(i, var);
		}
	}

	void Serializer::extractFromJson(rttr::variant& val, rapidjson::Value& jsonValue, fse::Scene* scene)
	{

		if (jsonValue.GetType() == rapidjson::kArrayType)
		{
			arrayFromJson(val, jsonValue, scene);
		}
		else if (jsonValue.GetType() == rapidjson::kObjectType)
		{
			extractObjectFromJson(val, jsonValue, scene);
		}
		else
		{
			val = extractBasicTypes(jsonValue);
		}
	}

	rttr::variant Serializer::extractBasicTypes(rapidjson::Value& jsonValue)
	{
		switch (jsonValue.GetType())
		{
			case rapidjson::kStringType:
				return std::string(jsonValue.GetString());
			case rapidjson::kNullType:    
				return rttr::variant();
			case rapidjson::kFalseType:
				return jsonValue.GetBool();
			case rapidjson::kTrueType:
				return jsonValue.GetBool();
			case rapidjson::kNumberType:
			{
				if (jsonValue.IsInt())
					return jsonValue.GetInt();
				if (jsonValue.IsFloat())
					return jsonValue.GetFloat();
				if (jsonValue.IsDouble())
					return jsonValue.GetDouble();
				if (jsonValue.IsUint())
					return jsonValue.GetUint();
				if (jsonValue.IsInt64())
					return jsonValue.GetInt64();
				if (jsonValue.IsUint64())
					return jsonValue.GetUint64();
				break;
			}
			default:
				return rttr::variant();
		}
		return rttr::variant();
	}

	void Serializer::extractObjectFromJson(rttr::variant& val, rapidjson::Value& jsonValue, fse::Scene* scene)
	{
		auto tmemb = jsonValue.FindMember("type");
		if (tmemb == jsonValue.MemberEnd())
			return;

		auto& tval = tmemb->value;

		if (tval.GetType() != rapidjson::kStringType)
			return;

		std::string typestr(tval.GetString());

		rttr::type type = rttr::type::get_by_name(typestr.data());
		
		for (auto& base : type.get_base_classes())
		{
			if (base == rttr::type::get<fse::FSEObject>())
			{
				FSEObjectFromJson(jsonValue, scene);
				return;
			}
		}

		auto ctors = type.get_constructors();
		if (ctors.size() > 0)
		{
			auto ctor = type.get_constructor({});
			
			if (ctor.is_valid())
			{
				val = ctor.invoke();
			} 
			else
			{
				ctor = type.get_constructor({ rttr::type::get<FSEObject*>() });
				if (ctor.is_valid())
				{
					if (scene->pending_object_spawns_.size() > 0)
					{
						auto it = scene->pending_object_spawns_.rbegin();
						FSEObject* ptr = (*it).get();
						val = ctor.invoke(ptr);
					}
				}
			}
		}


		if (typestr == std::string(rttr::type::get<sf::Vector2f>().get_name().data()))
		{
			sf::Vector2f vec;

			vec.x = jsonValue.FindMember("x")->value.GetFloat();
			vec.y = jsonValue.FindMember("y")->value.GetFloat();

			val = vec;

			return;
		}

		if (typestr == std::string(rttr::type::get<sf::Color>().get_name().data()))
		{
			sf::Color col;

			col.r = jsonValue.FindMember("r")->value.GetInt();
			col.g = jsonValue.FindMember("g")->value.GetInt();
			col.b = jsonValue.FindMember("b")->value.GetInt();
			col.a = jsonValue.FindMember("a")->value.GetInt();

			val = col;

			return;
		}

		if (typestr == std::string(rttr::type::get<sf::FloatRect>().get_name().data()))
		{
			sf::FloatRect rekt;

			rekt.left = jsonValue.FindMember("left")->value.GetFloat();
			rekt.top = jsonValue.FindMember("top")->value.GetFloat();
			rekt.width = jsonValue.FindMember("width")->value.GetFloat();
			rekt.height = jsonValue.FindMember("height")->value.GetFloat();

			val = rekt;
			return;
		}

		if (typestr == std::string(rttr::type::get<sf::IntRect>().get_name().data()))
		{
			sf::IntRect rekt;

			rekt.left = jsonValue.FindMember("left")->value.GetInt();
			rekt.top = jsonValue.FindMember("top")->value.GetInt();
			rekt.width = jsonValue.FindMember("width")->value.GetInt();
			rekt.height = jsonValue.FindMember("height")->value.GetInt();

			val = rekt;
			return;
		}

		if (val.is_valid())
		{
			for (auto& prop : type.get_properties())
			{
				if (prop.is_readonly())
					continue;

				auto jsonmemb = jsonValue.FindMember(prop.get_name().data());
				if (jsonmemb == jsonValue.MemberEnd())
					continue;

				rttr::variant var = prop.get_value(val);
				extractFromJson(var, jsonmemb->value, scene);

				if (var.get_type() != prop.get_type())
				{
					var.convert(prop.get_type());
				}

				if (var != prop.get_value(val))
					prop.set_value(val, var);

			}

		}
		
		return;
	}
}
