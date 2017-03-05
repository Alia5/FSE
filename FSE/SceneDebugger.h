#pragma once
#include <set>
#include <functional>
#include <rttr/type.h>
#include <unordered_map>
#include "Lights/Light.h"

namespace fse
{
	class Scene;
	class FSEObject;
	/*!
	 * \brief Scene debugger / editor \n
	 * 
	 * Supports nested and array-like types (std::vector, std::array, std::list,...) \n 
	 * Supports GUI editing bool, int, float, sf::Vector2f, sf::IntRect, sf::FloatRect and sf::Color
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
		 * \brief register or change edit function for type \n 
		 * 
		 * Usage: \n
		 *		registerItemEditFunc(rttr::type::get<int>(), [](rttr::property prop, rttr::instance* object){ \n 
		 *		your ImGui code here }); \n
		 * \param type rttr type 
		 * \param func property edit function
		 */
		void registerItemEditFunc(rttr::type type,
			std::function<void(rttr::property, rttr::instance*)> func);

		static std::unordered_map<rttr::type,
			std::function<void(rttr::property, rttr::instance*)>> CreateDefaultItemEditMap();

		/*!
		 * \brief enables "To mouse pos" button for sf::Vector2f types
		 */
		void enableVector2fToMousePos();

	private:
		void ShowSceneStatus() const;
		void ShowObjectList();
		void ShowMouseTools();
		void ShowObjectEditor();


		void ShowObjectEditorItems(rttr::type type, rttr::instance* object);

		Scene* scene_ = nullptr;
		int selected_id_ = -1;
		bool mouse_selection_mode_ = false;
		std::set<FSEObject*, std::function<bool(FSEObject*, FSEObject*)>> objects_;

		std::unordered_map<rttr::type,
			std::function<void(rttr::property, rttr::instance*)>> item_edit_funcs_;

		std::unordered_map<std::string, bool> vector_to_mouse_;

	};
}

