#pragma once

#include "Assets/Asset.h"
#include "PropertiesWindow/IPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class Button;
	class EditBox;
	class Image;
	class Label;
	class Widget;
}

namespace Editor
{
	class IAssetPropertiesViewer;

	class AssetsPropertiesViewer: public IPropertiesViewer
	{
	public:
		// Default constructor
		AssetsPropertiesViewer();

		// Destructor
		~AssetsPropertiesViewer();

		// Returns viewing object type
		const Type* GetViewingObjectType() const override;

		void AddAssetViewer(IAssetPropertiesViewer* viewer);

		IOBJECT(AssetsPropertiesViewer);

	protected:
		typedef Vector<IAssetPropertiesViewer*> AssetPropertiesViewersVec;
		typedef Vector<AssetRef*> AssetsVec;
		typedef Dictionary<const Type*, AssetPropertiesViewersVec> TypeAssetViewersDict;

		AssetsVec                 mTargetAssets;                 // Current target assets

		IAssetPropertiesViewer*   mCurrentViewer = nullptr;      // Current asset viewer
		AssetPropertiesViewersVec mAvailableAssetViewers;        // Available asset viewers

	protected:
		// Sets target objects
		void SetTargets(const Vector<IObject*> targets);

		// Enable viewer event function
		void OnEnabled();

		// Disable viewer event function
		void OnDisabled();

		// Updates viewer
		void Update(float dt);

		// Draws something
		void Draw();

		// Initializes head widget
		void InitializeHeadWidget();

		// It is called when asset name edit box was changed
		void OnNameTextEditChanged(const WString& value);
	};
}

CLASS_BASES_META(Editor::AssetsPropertiesViewer)
{
	BASE_CLASS(Editor::IPropertiesViewer);
}
END_META;
CLASS_FIELDS_META(Editor::AssetsPropertiesViewer)
{
	PROTECTED_FIELD(mTargetAssets);
	PROTECTED_FIELD(mCurrentViewer);
	PROTECTED_FIELD(mAvailableAssetViewers);
}
END_META;
CLASS_METHODS_META(Editor::AssetsPropertiesViewer)
{

	PUBLIC_FUNCTION(const Type*, GetViewingObjectType);
	PUBLIC_FUNCTION(void, AddAssetViewer, IAssetPropertiesViewer*);
	PROTECTED_FUNCTION(void, SetTargets, const Vector<IObject*>);
	PROTECTED_FUNCTION(void, OnEnabled);
	PROTECTED_FUNCTION(void, OnDisabled);
	PROTECTED_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, Draw);
	PROTECTED_FUNCTION(void, InitializeHeadWidget);
	PROTECTED_FUNCTION(void, OnNameTextEditChanged, const WString&);
}
END_META;
