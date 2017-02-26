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
	class SceneDebugger
	{
	public:
		SceneDebugger();
		explicit SceneDebugger(Scene* scene);

		void Update();

		static void RegisterItemEditFunc(rttr::type type,
			std::function<void(rttr::property, rttr::instance*)> func);

		static std::unordered_map<rttr::type,
			std::function<void(rttr::property, rttr::instance*)>> CreateDefaultItemEditMap();

	private:
		void ShowSceneStatus() const;
		void ShowObjectList();
		void ShowObjectEditor();

		void ShowObjectEditorItems(rttr::type type, rttr::instance* object) const;

		static std::unordered_map<rttr::type, 
				std::function<void(rttr::property, rttr::instance*)>> item_edit_funcs_;

		Scene* scene_ = nullptr;
		int selected_id_ = -1;
		std::set<FSEObject*, std::function<bool(FSEObject*, FSEObject*)>> objects_;



	};
}

