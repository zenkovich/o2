#include "o2/Scene/SceneLayersList.h"
#include "o2Editor/Core/Properties/IPropertyField.h"
#include "o2/Utils/Ref.h"

namespace o2
{
	class Button;
	class ContextMenu;
	class EditBox;
	class Widget;
}

namespace Editor
{
	// ---------------------------------
	// Editor scene layers list property
	// // ------------------------------
	class SceneLayersListProperty: public TPropertyField<SceneLayersList>
	{
	public:
		// Default constructor
		SceneLayersListProperty();

		// Copy constructor
		SceneLayersListProperty(const SceneLayersListProperty& other);

		// Copy operator
		SceneLayersListProperty& operator=(const SceneLayersListProperty& other);

		SERIALIZABLE(SceneLayersListProperty);

	protected:
		Ref<Button> mPropertyButton;       // Property box with layers' names
		Ref<Text> mPropertyText;           // Text layer inside property box

		Ref<ContextMenu> mLayersContext;   // Layers list toggles context

	protected:
		// Updates value view
		void UpdateValueView() override;

		// Searches controls widgets and layers and initializes them
		void InitializeControls();

		// Updates context layers list and opens
		void OpenContext();
	};
}

// --- META ---

CLASS_BASES_META(Editor::SceneLayersListProperty)
{
    BASE_CLASS(Editor::TPropertyField<SceneLayersList>);
}
END_META;
CLASS_FIELDS_META(Editor::SceneLayersListProperty)
{
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Button>()).NAME(mPropertyButton);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<Text>()).NAME(mPropertyText);
    FIELD().PROTECTED().DEFAULT_VALUE(mmake<ContextMenu>()).NAME(mLayersContext);
}
END_META;
CLASS_METHODS_META(Editor::SceneLayersListProperty)
{

    FUNCTION().PUBLIC().CONSTRUCTOR();
    FUNCTION().PUBLIC().CONSTRUCTOR(const SceneLayersListProperty&);
    FUNCTION().PROTECTED().SIGNATURE(void, UpdateValueView);
    FUNCTION().PROTECTED().SIGNATURE(void, InitializeControls);
    FUNCTION().PROTECTED().SIGNATURE(void, OpenContext);
}
END_META;
// --- END META ---