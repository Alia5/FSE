#pragma once
#include <set>
#include <functional>
#include <rttr/type.h>
#include <unordered_map>
#include "Lights/Light.h"
#include <vector>
#include <string>

namespace fse
{
	class Scene;
	class FSEObject;
	/*!
	 * \brief Scene debugger / editor.
	 * 
	 * Supports nested and array-like types (std::vector, std::array, std::list,...) \n 
	 * Supports GUI editing for bool, int, float, sf::Vector2f, sf::IntRect, sf::FloatRect, sf::Color and std::strings with up to 256 characters \n
	 * \n
	 * However, you can register support for more types or change how they are handled using \ref registerItemEditFunc() \n
	 * If you additionally wish to create objects with, also register a default value for the new type \ref registerDefaultValue() \n
	 */
	class SceneDebugger
	{
	public:
		SceneDebugger();
		explicit SceneDebugger(Scene* scene);

		/*!
		 * \brief update Scene debugger
		 */
		void update();

		/*!
		 * \brief register or change edit function for type.
		 * 
		 * Usage: \n
		 * \code
		 *		registerItemEditFunc(rttr::type::get<int>(), [](rttr::variant& variant, std::string name, rttr::instance* instance){
		 *		std::string propname(name + "##" + std::to_string(reinterpret_cast<int>(instance)));
		 *		int val = variant.convert<int>;
		 *		//your ImGui code here: EXAMPLE
		 *		if (ImGui::InputInt(propname.data(), &val))
		 *			variant = rttr::variant(int); //don't forget to change the variant object... 
		 *		//
		 *		});
		 *	\endcode
		 * \param type rttr type 
		 * \param func property edit function
		 */
		void registerItemEditFunc(rttr::type type,
			std::function<void(rttr::variant&, std::string, rttr::instance*)> func);


		void registerDefaultValue(rttr::type type, const rttr::variant& variant);

		static std::unordered_map<rttr::type,
			std::function<void(rttr::variant&, std::string, rttr::instance*)>> CreateDefaultItemEditMap();

		/*!
		 * \brief enables "To mouse pos" button for sf::Vector2f types.
		 */
		void enableVector2fToMousePos();

	private:
		void ShowSceneStatus();
		void ShowObjectList();
		void ShowMouseTools();

		void ShowObjectSpawner();

		void ShowObjectEditor();


		void ShowObjectEditorItems(rttr::type type, rttr::instance* object);

		void CreateDefaultDefaultValueMap();

		Scene* scene_ = nullptr;
		int selected_id_ = -1;
		bool mouse_selection_mode_ = false;
		bool mouse_spawn_mode_ = false;
		bool mouse_spawn_until_right = false;
		bool mouse_spawn_timeout_ = false;
		std::set<FSEObject*, std::function<bool(FSEObject*, FSEObject*)>> objects_;

		std::unordered_map<rttr::type,
			std::function<void(rttr::variant&, std::string, rttr::instance*)>> item_edit_funcs_;

		std::unordered_map<std::string, bool> vector_to_mouse_;

		std::unordered_map<std::string, std::vector<rttr::variant>> spawn_args_;
		std::unordered_map<rttr::type, rttr::variant> default_vals_;

		std::string serialize_file_name_ = "debug_level.json";

	};
}

