#pragma once
#include <set>
#include <functional>
#include <rttr/type.h>
#include <unordered_map>

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

		void RegisterItemEditFunc(rttr::type type,
			std::function<void(rttr::property, FSEObject*)> func);

		static std::unordered_map<rttr::type,
			std::function<void(rttr::property, FSEObject*)>> CreateDefaultItemEditMap();

	private:
		void ShowObjectList();
		void ShowObjectEditor();

		void ShowObjectEditorItems(rttr::type type, FSEObject* object);

		std::unordered_map<rttr::type, 
				std::function<void(rttr::property, FSEObject*)>> item_edit_funcs_;

		Scene* scene_ = nullptr;
		int selected_id_ = -1;
		std::set<FSEObject*, std::function<bool(FSEObject*, FSEObject*)>> objects_;




	};
}

