#pragma once

#include "Utils/Serializable.h"

using namespace o2;

namespace Editor
{
	class IAction: public ISerializable
	{
	public:
		virtual ~IAction() {}

		virtual String GetName() const { return "Unknown"; }
		virtual void Redo() {}
		virtual void Undo() {}

		SERIALIZABLE(IAction);
	};
}