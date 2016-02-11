#pragma once

#include "Utils/Serialization.h"

using namespace o2;

class IEditorAction: public ISerializable
{
public:
	virtual ~IEditorAction() {}

	virtual String GetName() const { return "Unknown"; }
	virtual void Redo() {}
	virtual void Undo() {}

	SERIALIZABLE(IEditorAction);
};
