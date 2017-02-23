#include "SceneDebugger.h"
#include "FSEObject/FSEObject.h"
//#include "../imgui-1.49/imgui.h"
#include "../FSE/FSE-ImGui/imgui-colorpicker.h"
#include "../imgui-1.49/imgui_internal.h"

namespace fse
{
	SceneDebugger::SceneDebugger()
	{

	}

	SceneDebugger::SceneDebugger(Scene* scene) : scene_(scene)
	{
		auto comp = [](FSEObject* lhs, FSEObject* rhs)
		{
			return lhs->getID() < rhs->getID();
		};
		objects_ = std::set<FSEObject*, std::function<bool(FSEObject*, FSEObject*)>>(comp);

		item_edit_funcs_ = CreateDefaultItemEditMap();
	}

	void SceneDebugger::Update()
	{
		if (scene_ == nullptr)
			return;
		objects_.clear();
		for (const auto& object : *scene_->getFSEObjects())
		{
			objects_.insert(object.get());
		}

		ImGui::Begin("SceneDebugger##MainMenu");

		ShowObjectList();

		if (ImGui::CollapsingHeader("Object Editor##SceneDebugger"))
		{
			ShowObjectEditor();
		}

		ImGui::End();
	}

	void SceneDebugger::RegisterItemEditFunc(rttr::type type,
		std::function<void(rttr::property, FSEObject*)> func)
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
				while ((pos = nodename.find("class "), pos) != std::wstring::npos)
				{
					nodename.erase(pos, 6);
				}
				if (ImGui::Selectable(nodename.data(), id == selected_id_))
				{
					selected_id_ = id;
				}
			}


			ImGui::EndChild();
		}
	}

	void SceneDebugger::ShowObjectEditor()
	{
		ImGui::BeginChild("##ObjectEditor##SceneDebugger", ImVec2(0, ImGui::GetCurrentWindow()->Size.y / 2.f), true);
		auto it = std::find_if(objects_.begin(), objects_.end(), [this](FSEObject* object){ return object->getID() == selected_id_;});
		if (it == objects_.end())
		{
			ImGui::Text("Object: <None>");
			ImGui::Separator();
			ImGui::EndChild();
			return;
		} 
		ImGui::PushItemWidth(ImGui::GetCurrentWindow()->Size.x - 150);
		rttr::type type = rttr::type::get(**it);
		ImGui::Text(std::string("Object: " + std::string(type.get_name().data())).data());
		ImGui::Separator();

		ShowObjectEditorItems(type, *it);

		ImGui::Separator();
		if(ImGui::Button("Destroy##SceneDebugger"))
		{
			type.invoke("destroy",*it,{});
		}
		ImGui::PopItemWidth();
		ImGui::EndChild();
	}

	void SceneDebugger::ShowObjectEditorItems(rttr::type type, FSEObject* object)
	{
		for (auto& prop : type.get_properties())
			if (item_edit_funcs_.count(prop.get_type()))
				item_edit_funcs_[prop.get_type()](prop,object);
	}



	std::unordered_map<rttr::type,
		std::function<void(rttr::property, FSEObject*)>> SceneDebugger::CreateDefaultItemEditMap()
	{
		std::unordered_map<rttr::type,
			std::function<void(rttr::property, FSEObject*)>> item_edit_map_;

		auto boolFunc = [](rttr::property prop, FSEObject* object)
		{
			std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(object->getID()));
			bool val = prop.get_value(*object).convert<bool>();
			if (ImGui::Checkbox(propname.data(), &val))
				if (!prop.is_readonly())
					prop.set_value(*object, val);
		};
		item_edit_map_[rttr::type::get<bool>()] = boolFunc;

		auto intFunc = [](rttr::property prop, FSEObject* object)
		{
			std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(object->getID()));
			int val = prop.get_value(*object).convert<int>();
			if (ImGui::InputInt(propname.data(), &val))
				if (!prop.is_readonly())
					prop.set_value(*object, val);
		};
		item_edit_map_[rttr::type::get<int>()] = intFunc;

		auto floatFunc = [](rttr::property prop, FSEObject* object)
		{
			std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(object->getID()));
			float val = prop.get_value(*object).convert<float>();
			if (ImGui::InputFloat(propname.data(), &val))
				if (!prop.is_readonly())
					prop.set_value(*object, val);
		};
		item_edit_map_[rttr::type::get<float>()] = floatFunc;

		auto Vector2fFunc = [](rttr::property prop, FSEObject* object)
		{
			std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(object->getID()));
			sf::Vector2f val = prop.get_value(*object).convert<sf::Vector2f>();
			float arr[2] = {val.x, val.y};

			if (ImGui::InputFloat2(propname.data(), &arr[0]))
			{
				if (!prop.is_readonly())
				{
					val = {arr[0], arr[1]};
					prop.set_value(*object, val);
				}
			}
		};
		item_edit_map_[rttr::type::get<sf::Vector2f>()] = Vector2fFunc;

		auto IntRectFunc = [](rttr::property prop, FSEObject* object)
		{
			std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(object->getID()));
			sf::IntRect val = prop.get_value(*object).convert<sf::IntRect>();
			int arr[4] = {val.left, val.top, val.width, val.height};

			if (ImGui::InputInt4(propname.data(), &arr[0]))
			{
				if (!prop.is_readonly())
				{
					val = { arr[0], arr[1], arr[2], arr[3] };
					prop.set_value(*object, val);
				}
			}
		};
		item_edit_map_[rttr::type::get<sf::IntRect>()] = IntRectFunc;

		auto FloatRectFunc = [](rttr::property prop, FSEObject* object)
		{
			std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(object->getID()));
			sf::FloatRect val = prop.get_value(*object).convert<sf::FloatRect>();
			float arr[4] = { val.left, val.top, val.width, val.height };

			if (ImGui::InputFloat4(propname.data(), &arr[0]))
			{
				if (!prop.is_readonly())
				{
					val = { arr[0], arr[1], arr[2], arr[3] };
					prop.set_value(*object, val);
				}
			}

		};
		item_edit_map_[rttr::type::get<sf::FloatRect>()] = FloatRectFunc;

		auto ColorFunc = [](rttr::property prop, FSEObject* object)
		{
			std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(object->getID()));

			sf::Color val = prop.get_value(*object).convert<sf::Color>();
			float color[4] = { (val.r / 255.f), (val.g / 255.f), (val.b / 255.f), (val.a / 255.f) };

			if (ImGui::ColorEdit4(propname.data(), color))
			{
				if (!prop.is_readonly())
				{
					val = { static_cast<sf::Uint8>(color[0] * 255.f),
						static_cast<sf::Uint8>(color[1] * 255.f) ,
						static_cast<sf::Uint8>(color[2] * 255.f) ,
						static_cast<sf::Uint8>(color[3] * 255.f) };
					prop.set_value(*object, val);
				}
			}
			if (ImGui::TreeNode(std::string("Colorpicker##" + std::to_string(object->getID())).data()))
			{
				ImGui::PushItemWidth(200);
				if (ImGui::ColorPicker4(std::string(propname + "##Picker" + std::to_string(object->getID())).data(),
					color, ImGui::ImGuiColorEditFlags_NoSliders))
				{
					if (!prop.is_readonly())
					{
						val = { static_cast<sf::Uint8>(color[0] * 255.f),
							static_cast<sf::Uint8>(color[1] * 255.f) ,
							static_cast<sf::Uint8>(color[2] * 255.f) ,
							static_cast<sf::Uint8>(color[3] * 255.f) };
						prop.set_value(*object, val);
					}
				}
				ImGui::PopItemWidth();
				ImGui::TreePop();
			}

		};
		item_edit_map_[rttr::type::get<sf::Color>()] = ColorFunc;


		return item_edit_map_;
	}
}
