#include "SceneDebugger.h"
#include "FSEObject/FSEObject.h"
//#include "../imgui-1.49/imgui.h"
#include "../FSE/FSE-ImGui/imgui-colorpicker.h"
#include "../imgui-1.49/imgui_internal.h"

#include <iomanip>
#include <sstream>

namespace fse
{

	std::unordered_map<rttr::type,
		std::function<void(rttr::property, rttr::instance*)>> SceneDebugger::item_edit_funcs_;

	SceneDebugger::SceneDebugger()
	{

	}

	SceneDebugger::SceneDebugger(Scene* scene) : scene_(scene)
	{

		objects_ = std::set<FSEObject*, std::function<bool(FSEObject*, FSEObject*)>>(
			[](FSEObject* lhs, FSEObject* rhs) {return lhs->getID() < rhs->getID(); });

		if (item_edit_funcs_.size() == 0)
			item_edit_funcs_ = CreateDefaultItemEditMap();
	}

	void SceneDebugger::Update()
	{
		if (scene_ == nullptr)
			return;

		objects_.clear();
		for (const auto& object : *scene_->getFSEObjects())
			objects_.insert(object.get());

		ImGui::Begin("SceneDebugger##MainMenu");

		ShowSceneStatus();

		ShowObjectList();

		if (ImGui::CollapsingHeader("Object Editor##SceneDebugger"))
			ShowObjectEditor();

		ImGui::End();
	}

	void SceneDebugger::RegisterItemEditFunc(rttr::type type,
		std::function<void(rttr::property, rttr::instance*)> func)
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

		ShowObjectEditorItems(type, &rttr::instance(*it));

		for (auto& method : type.get_methods())
		{
			if (method.get_name().compare("destroy") == 0)
			{
				ImGui::Separator();
				if (ImGui::Button("Destroy##SceneDebugger"))
				{
					type.invoke("destroy", *it, {}); //with fire
				}
			}
		}

		ImGui::PopItemWidth();
		ImGui::EndChild();
	}

	void SceneDebugger::ShowObjectEditorItems(rttr::type type, rttr::instance* object) const
	{
		for (auto& prop : type.get_properties())
		{
			if (item_edit_funcs_.count(prop.get_type()))
				item_edit_funcs_[prop.get_type()](prop, object);
			else if (prop.get_type().get_properties().size() > 0)
			{
				std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(reinterpret_cast<int>(object)));
				if (ImGui::TreeNode(propname.data()))
				{
					auto val = prop.get_value(*object);
					auto inst = rttr::instance(val);
					ShowObjectEditorItems(prop.get_type(), &inst);
					prop.set_value(*object, val);

					ImGui::TreePop();
				}
			}
		}
	}

	std::unordered_map<rttr::type,
		std::function<void(rttr::property, rttr::instance*)>> SceneDebugger::CreateDefaultItemEditMap()
	{
		std::unordered_map<rttr::type,
			std::function<void(rttr::property, rttr::instance*)>> item_edit_map_;

		item_edit_map_[rttr::type::get<bool>()] = [](rttr::property prop, rttr::instance* object)
		{
			std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(reinterpret_cast<int>(object)));
			bool val = prop.get_value(*object).convert<bool>();
			if (ImGui::Checkbox(propname.data(), &val))
				if (!prop.is_readonly())
					prop.set_value(*object, val);
		};

		item_edit_map_[rttr::type::get<int>()] = [](rttr::property prop, rttr::instance* object)
		{
			std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(reinterpret_cast<int>(object)));
			int val = prop.get_value(*object).convert<int>();
			if (ImGui::InputInt(propname.data(), &val))
				if (!prop.is_readonly())
					prop.set_value(*object, val);
		};

		item_edit_map_[rttr::type::get<float>()] = [](rttr::property prop, rttr::instance* object)
		{
			std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(reinterpret_cast<int>(object)));
			float val = prop.get_value(*object).convert<float>();
			if (ImGui::InputFloat(propname.data(), &val))
				if (!prop.is_readonly())
					prop.set_value(*object, val);
		};

		item_edit_map_[rttr::type::get<sf::Vector2f>()] = [](rttr::property prop, rttr::instance* object)
		{
			std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(reinterpret_cast<int>(object)));
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

		item_edit_map_[rttr::type::get<sf::IntRect>()] = [](rttr::property prop, rttr::instance* object)
		{
			std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(reinterpret_cast<int>(object)));
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

		item_edit_map_[rttr::type::get<sf::FloatRect>()] = [](rttr::property prop, rttr::instance* object)
		{
			std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(reinterpret_cast<int>(object)));
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

		item_edit_map_[rttr::type::get<sf::Color>()] = [](rttr::property prop, rttr::instance* object)
		{
			std::string propname(std::string(prop.get_name().data()) + "##" + std::to_string(reinterpret_cast<int>(object)));

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
			if (ImGui::TreeNode(std::string("Colorpicker##" + std::to_string(reinterpret_cast<int>(object))).data()))
			{
				ImGui::PushItemWidth(200);
				if (ImGui::ColorPicker4(std::string(propname + "##Picker" + std::to_string(reinterpret_cast<int>(object))).data(),
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

		return item_edit_map_;
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
