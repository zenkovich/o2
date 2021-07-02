#pragma once
#include "o2/Utils/Types/StringDef.h"

using namespace o2;

namespace Editor
{
	class SceneEditorLayer
	{
	public:
		virtual void DrawScene() {}
		virtual void DrawOverScene() {}
		virtual void Update(float dt) {}

		virtual int GetOrder() const { return 0; }

		virtual bool IsEnabled() const { return true; }

		virtual const String& GetName() const = 0;
		virtual const String& GetIconName() const = 0;
	};
}
