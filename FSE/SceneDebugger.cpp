#include "SceneDebugger.h"
#include "FSEObject/FSEObject.h"
//#include "../imgui-1.49/imgui.h"
#include "../FSE/FSE-ImGui/imgui-colorpicker.h"
#include "../imgui-1.49/imgui_internal.h"

#include "../FSE/Application.h"

#include <sstream>


namespace fse
{

	SceneDebugger::SceneDebugger()
	{
		if (item_edit_funcs_.size() == 0)
			item_edit_funcs_ = CreateDefaultItemEditMap();
	}

	SceneDebugger::SceneDebugger(Scene* scene) : scene_(scene)
	{

		objects_ = std::set<FSEObject*, std::function<bool(FSEObject*, FSEObject*)>>(
			[](FSEObject* lhs, FSEObject* rhs) {return lhs->getID() < rhs->getID(); });

		if (item_edit_funcs_.size() == 0)
			item_edit_funcs_ = CreateDefaultItemEditMap();

		CreateDefaultDefaultValueMap();
	}

	void SceneDebugger::update()
	{
		if (scene_ == nullptr)
			return;

		objects_.clear();
		for (const auto& object : *scene_->getFSEObjects())
			objects_.insert(object.get());

		ImGui::Begin("SceneDebugger##MainMenu");

		ShowSceneStatus();

		ShowMouseTools();

		ShowObjectSpawner();

		ShowObjectList();

		if (ImGui::CollapsingHeader("Object Editor##SceneDebugger"))
			ShowObjectEditor();

		ImGui::End();
	}

	void SceneDebugger::registerItemEditFunc(rttr::type type,
		std::function<void(rttr::variant&, std::string, rttr::instance*)> func)
	{
		item_edit_funcs_[type] = func;
	}
	
	void SceneDebugger::ShowObjectList()
	{
		if (ImGui::CollapsingHeader("Subscene Objects##SceneDebugger"))
		{
			ImGui::BeginChild("##Objects##SceneDebugger", ImVec2(0, 250), true);
			int id;
			for (const auto& object : objects_)
			{
				id = object->getID();
				rttr::type type = rttr::type::get(*object);
				std::string nodename = std::string(type.get_name().data()) + "##SceneDebugger" + std::to_string(id);
				int pos;
				while ((pos = nodename.find("class "), pos) != std::string::npos)
					nodename.erase(pos, 6);
				if (ImGui::Selectable(nodename.data(), id == selected_id_))
					selected_id_ = id;
			}
			ImGui::EndChild();
		}
	}

	void SceneDebugger::ShowMouseTools()
	{
		ImGui::Checkbox("Get mouse over Object##SceneDebugger", &mouse_selection_mode_);

		if (mouse_selection_mode_)
		{
			ImGui::Text("\"getAABBs()\" have to be overridden properly!");
			ImGui::Text("Leftclick to end");
		}

		if (mouse_selection_mode_)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				mouse_selection_mode_ = false;
				return;
			}

			selected_id_ = -1;

			//use z-order sorted list from scene directly
			//reverse iter because of z-ordering...
			auto objects = scene_->getFSEObjects();
			for (auto it = objects->rbegin(); it != objects->rend(); ++it)
			{
				auto aabbs = it->get()->GetAABBs();
				sf::RenderWindow* window = scene_->getApplication()->getWindow();
				if (aabbs.contains(
					window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
				{
					selected_id_ = it->get()->getID();
					break;
				}
			}
		}

		ImGui::Separator();

	}

	void SceneDebugger::ShowObjectSpawner() 
	{
		if (ImGui::CollapsingHeader("Object Spawner##SceneDebugger"))
		{
			ImGui::BeginChild("##Object Spawner##SceneDebugger", ImVec2(0, 250), true);

			auto types = rttr::type::get<fse::FSEObject>().get_derived_classes();	
			for (auto& type : types)
			{
				auto ctors = type.get_constructors();
				if (!ctors.empty())
				{
					std::string treename = std::string(type.get_name().data()) + "##ObjectSpawner";
					if (ImGui::TreeNode(treename.data()))
					{
						for (auto& ctor : ctors)
						{
							auto infos = ctor.get_parameter_infos();					

							std::string nodename("");
							for (auto& info : infos)
							{
								nodename += info.get_type().get_name().data();
								nodename += " ";
								nodename += info.get_name().data();
								nodename += ", ";
							}
							int pos;
							while ((pos = nodename.find("class "), pos) != std::string::npos)
								nodename.erase(pos, 6);

							pos = nodename.find(", ");
							nodename.erase(0, pos+2);
							nodename += "##" + std::string(type.get_name().data());
							if (ImGui::TreeNode(nodename.data()))
							{
								auto args = spawn_args_[nodename];
								if (args.empty())
								{
									for (auto& info : infos)
									{
										auto t = info.get_type();
										auto var = default_vals_[t];
										args.push_back(var);
									}
								}

								if (infos.size() == args.size())
								{
									ImGui::PushItemWidth(ImGui::GetCurrentWindow()->Size.x - 350);
									auto arg_it = args.begin();
									auto info_it = infos.begin();
									while (arg_it != args.end())
									{
										auto t = arg_it->get_type();
										if (t != rttr::type::get<fse::Scene*>())
										{
											auto inst = rttr::instance(*arg_it);
											if (item_edit_funcs_.count(t))
												item_edit_funcs_[t](*arg_it, std::string(info_it->get_name().data()), &inst);
										}
										++arg_it;
										++info_it;
									}
									spawn_args_[nodename] = args;
									ImGui::PopItemWidth();
								}

								if (ImGui::Button(std::string(std::string(" SPAWN ##") + nodename).data()))
								{
									std::vector<rttr::argument> arguvec;
									for (auto & arg : args)
										arguvec.push_back(arg);
									if (arguvec.size() > 0)
										ctor.invoke_variadic(arguvec);
								}

								ImGui::Separator();
								ImGui::TreePop();
							}
						}

						ImGui::Separator();
						ImGui::TreePop();
					}
				}
			}
			ImGui::EndChild();
		}
	}

	void SceneDebugger::ShowObjectEditor()
	{
		ImGui::BeginChild("##ObjectEditor##SceneDebugger", ImVec2(0,0), true);
		auto it = std::find_if(objects_.begin(), objects_.end(), [this](FSEObject* object){ return object->getID() == selected_id_;});
		if (it == objects_.end())
		{
			ImGui::Text("Object: <None>");
			ImGui::Separator();
			ImGui::EndChild();
			return;
		} 
		ImGui::PushItemWidth(ImGui::GetCurrentWindow()->Size.x - 250);
		rttr::type type = rttr::type::get(**it);
		ImGui::Text(std::string("Object: " + std::string(type.get_name().data())).data());
		ImGui::Separator();

		auto inst = rttr::instance(*it);
		ShowObjectEditorItems(type, &inst);

		for (auto& method : type.get_methods())
		{
			if (method.get_name().compare("destroy") == 0)
			{
				ImGui::Separator();
				if (ImGui::Button(" DESTROY ##SceneDebugger"))
				{
					type.invoke("destroy", *it, {}); //with fire
				}
			}
		}

		ImGui::PopItemWidth();
		ImGui::EndChild();
	}

	void SceneDebugger::ShowObjectEditorItems(rttr::type type, rttr::instance* object)
	{
		if (type.is_pointer())
		{
			auto t = type.get_raw_type();
			auto tdclasses = t.get_derived_classes();

			//search the most derived valid type
			auto valid_t = t;
			for (auto it = tdclasses.begin(); it != tdclasses.end(); ++it)
			{
				bool valid = true;
				for (auto& prop : it->get_properties())
				{
					if (!prop.get_value(*object).is_valid())
					{
						valid = false;
						break;
					} 
				}
				if (!valid)
					break;
				valid_t = *it;
			}
			ImGui::Text(valid_t.get_name().data());
			ShowObjectEditorItems(valid_t, object);
			return;
		}

		for (auto& prop : type.get_properties())
		{
			if (item_edit_funcs_.count(prop.get_type()))
			{
				auto val = prop.get_value(*object);
				auto bkup = val;
				std::string name(prop.get_name().data());
				if (prop.is_readonly())
					name += " -READONLY-";
				item_edit_funcs_[prop.get_type()](val, name, object);
				if (!prop.is_readonly() && bkup != val)
					prop.set_value(*object, val);
			}
			else if (prop.get_type().get_properties().size() > 0)
			{
				for (auto& pr : prop.get_type().get_properties())
				{
					if (item_edit_funcs_.count(pr.get_type()))
					{
						std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(reinterpret_cast<int>(object)));
						if (ImGui::TreeNode(propname.data()))
						{
							auto val = prop.get_value(*object);
							auto bkup = val;
							auto inst = rttr::instance(val);
							ShowObjectEditorItems(prop.get_type(), &inst);
							if (!prop.is_readonly() && bkup != val)
								prop.set_value(*object, val);

							ImGui::Separator();
							ImGui::TreePop();
						}
						break;
					}
				}
			} else if (prop.get_type().is_array())	{
						
				std::string propname(std::string(prop.get_name().data()) + "[]");
				if (prop.is_readonly())
					propname += " -READONLY-";
				propname += "##" + std::to_string(reinterpret_cast<int>(object));
				if (ImGui::TreeNode(propname.data()))
				{
					auto val = prop.get_value(*object);
					rttr::variant_array_view arr = val.create_array_view();
					size_t sz = arr.get_size();
					for (size_t i = 0; i < sz; i++)
					{
						std::string propn(std::string(prop.get_name().data()) + "[" + std::to_string(i) + "]");
						if (prop.is_readonly())
							propn += " -READONLY-";
						propn += "##" + std::to_string(reinterpret_cast<int>(object));
						if (ImGui::TreeNode(propn.data()))
						{
							auto v = arr.get_value(i);
							auto bkup = v;
							auto t = v.get_type();
							auto inst = rttr::instance(v);
							ShowObjectEditorItems(t, &inst);
							if (bkup != val)
								arr.set_value(i, v);

							ImGui::Separator();
							ImGui::TreePop();
						}
					}
					if (!prop.is_readonly())
						prop.set_value(*object, val);
						
					ImGui::Separator();
					ImGui::TreePop();
				}
			} else {
				std::string propname("Unsupported type: " 
					+ std::string(prop.get_type().get_name().data()) + "; "
					+ std::string(prop.get_name().data()));
				ImGui::Text(propname.data());
			}
		}
	}

	std::unordered_map<rttr::type,
		std::function<void(rttr::variant&, std::string, rttr::instance*)>> SceneDebugger::CreateDefaultItemEditMap()
	{
		std::unordered_map<rttr::type,
			std::function<void(rttr::variant&, std::string, rttr::instance*)>> item_edit_map_;

		item_edit_map_[rttr::type::get<bool>()] = [](rttr::variant& variant, std::string name, rttr::instance* instance)
		{
			std::string propname(name + "##" + std::to_string(reinterpret_cast<int>(instance)));
			bool val = variant.convert<bool>();
			if (ImGui::Checkbox(propname.data(), &val))
				variant = rttr::variant(val);
		};

		item_edit_map_[rttr::type::get<int>()] = [](rttr::variant& variant, std::string name, rttr::instance* instance)
		{
			std::string propname(name + "##" + std::to_string(reinterpret_cast<int>(instance)));
			int val = variant.convert<int>();
			if (ImGui::InputInt(propname.data(), &val))
				variant = rttr::variant(val);
		};

		item_edit_map_[rttr::type::get<float>()] = [](rttr::variant& variant, std::string name, rttr::instance* instance)
		{
			std::string propname(name + "##" + std::to_string(reinterpret_cast<int>(instance)));
			float val = variant.convert<float>();
			if (ImGui::InputFloat(propname.data(), &val))
				variant = rttr::variant(val);
		};

		item_edit_map_[rttr::type::get<sf::Vector2f>()] = [](rttr::variant& variant, std::string name, rttr::instance* instance)
		{
			std::string propname(name + "##" + std::to_string(reinterpret_cast<int>(instance)));
			sf::Vector2f val = variant.convert<sf::Vector2f>();
			float arr[2] = {val.x, val.y};

			if (ImGui::InputFloat2(propname.data(), &arr[0]))
			{
				val = {arr[0], arr[1]};
				variant = rttr::variant(val);
			}
		};

		item_edit_map_[rttr::type::get<sf::IntRect>()] = [](rttr::variant& variant, std::string name, rttr::instance* instance)
		{
			std::string propname(name + "##" + std::to_string(reinterpret_cast<int>(instance)));
			sf::IntRect val = variant.convert<sf::IntRect>();
			int arr[4] = {val.left, val.top, val.width, val.height};

			if (ImGui::InputInt4(propname.data(), &arr[0]))
			{
				val = { arr[0], arr[1], arr[2], arr[3] };
				variant = rttr::variant(val);
			}
		};

		item_edit_map_[rttr::type::get<sf::FloatRect>()] = [](rttr::variant& variant, std::string name, rttr::instance* instance)
		{
			std::string propname(name + "##" + std::to_string(reinterpret_cast<int>(instance)));
			sf::FloatRect val = variant.convert<sf::FloatRect>();
			float arr[4] = { val.left, val.top, val.width, val.height };

			if (ImGui::InputFloat4(propname.data(), &arr[0]))
			{
				val = { arr[0], arr[1], arr[2], arr[3] };
				variant = rttr::variant(val);
			}

		};

		item_edit_map_[rttr::type::get<sf::Color>()] = [](rttr::variant& variant, std::string name, rttr::instance* instance)
		{
			std::string propname(name + "##" + std::to_string(reinterpret_cast<int>(instance)));

			sf::Color val = variant.convert<sf::Color>();
			float color[4] = { (val.r / 255.f), (val.g / 255.f), (val.b / 255.f), (val.a / 255.f) };

			if (ImGui::ColorEdit4(propname.data(), color))
			{

					val = { static_cast<sf::Uint8>(color[0] * 255.f),
						static_cast<sf::Uint8>(color[1] * 255.f) ,
						static_cast<sf::Uint8>(color[2] * 255.f) ,
						static_cast<sf::Uint8>(color[3] * 255.f) };
				variant = rttr::variant(val);
			}
			if (ImGui::TreeNode(std::string("Colorpicker##" + std::to_string(reinterpret_cast<int>(instance))).data()))
			{
				ImGui::PushItemWidth(200);
				if (ImGui::ColorPicker4(std::string(propname + "##Picker" + std::to_string(reinterpret_cast<int>(instance))).data(),
					color, ImGui::ImGuiColorEditFlags_NoSliders))
				{

						val = { static_cast<sf::Uint8>(color[0] * 255.f),
							static_cast<sf::Uint8>(color[1] * 255.f) ,
							static_cast<sf::Uint8>(color[2] * 255.f) ,
							static_cast<sf::Uint8>(color[3] * 255.f) };
						variant = rttr::variant(val);
				}
				ImGui::PopItemWidth();
				ImGui::TreePop();
			}
		};

		return item_edit_map_;
	}

	void SceneDebugger::CreateDefaultDefaultValueMap()
	{
		default_vals_[rttr::type::get<fse::Scene*>()] = rttr::variant(scene_);
		default_vals_[rttr::type::get<bool>()] = rttr::variant(false);
		default_vals_[rttr::type::get<int>()] = rttr::variant(0);
		default_vals_[rttr::type::get<float>()] = rttr::variant(0.f);
		default_vals_[rttr::type::get<sf::Vector2f>()] = rttr::variant(sf::Vector2f(0.f, 0.f));
		default_vals_[rttr::type::get<sf::IntRect>()] = rttr::variant(sf::IntRect(0, 0, 0, 0));
		default_vals_[rttr::type::get<sf::FloatRect>()] = rttr::variant(sf::FloatRect(0.f, 0.f, 0.f, 0.f));
		default_vals_[rttr::type::get<sf::Color>()] = rttr::variant(sf::Color::White);
	}

	void SceneDebugger::enableVector2fToMousePos()
	{
		item_edit_funcs_[rttr::type::get<sf::Vector2f>()] = [this](rttr::variant& variant, std::string name, rttr::instance* instance)
		{
			std::string propname(name + "##" + std::to_string(reinterpret_cast<int>(instance)));
			sf::Vector2f val = variant.convert<sf::Vector2f>();
			float arr[2] = { val.x, val.y };

			if (ImGui::InputFloat2(propname.data(), &arr[0]))
			{
				val = { arr[0], arr[1] };
				variant = rttr::variant(val);
			}

			if (!vector_to_mouse_.count(propname))
				vector_to_mouse_[propname] = false;

			ImGui::SameLine();
			if (ImGui::Button(std::string("To Mouse position##" + propname).data()))
			{
				vector_to_mouse_[propname] = true;
				sf::RenderWindow* window = scene_->getApplication()->getWindow();
				sf::Mouse::setPosition(sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2), *window);
			}

			if (vector_to_mouse_[propname])
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					vector_to_mouse_[propname] = false;
					return;
				}
				ImGui::Text("Leftclick to end");

				sf::RenderWindow* window = scene_->getApplication()->getWindow();
				val = window->mapPixelToCoords(sf::Mouse::getPosition(*window)) * FSE_METERS_PER_PIXEL;
				variant = rttr::variant(val);
			}
		};
	}

	void SceneDebugger::ShowSceneStatus() const
	{
		std::stringstream stream;
		stream << "Scene: 0x" << std::hex << static_cast<void*>(scene_);
		ImGui::Text(stream.str().data());

		ImGui::Text(std::string("Object count: " + std::to_string(objects_.size())).data());
		bool b = scene_->isPaused();
		if (ImGui::Checkbox("Paused##SceneDebugger", &b))
		{
			scene_->setPaused(b);
		}
		ImGui::Separator();
	}
}
