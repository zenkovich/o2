#include "IAction.h"

CLASS_META(Editor::IAction)
{
	BASE_CLASS(o2::ISerializable);


	PUBLIC_FUNCTION(String, GetName);
	PUBLIC_FUNCTION(void, Redo);
	PUBLIC_FUNCTION(void, Undo);
}
END_META;
