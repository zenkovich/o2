#pragma once

#include "PropertiesWindow/IObjectPropertiesViewer.h"

using namespace o2;

namespace o2
{
	class Asset;
	class UIButton;
	class UIEditBox;
	class UIImage;
	class UILabel;
	class UIWidget;
}

namespace Editor
{
	class IAssetPropertiesViewer;

	class AssetsPropertiesViewer: public IObjectPropertiesViewer
	{
	public:
		// Default constructor
		AssetsPropertiesViewer();

		// Destructor
		~AssetsPropertiesViewer();

		// Returns viewing object type
		const Type* GetViewingObjectType() const;

		void AddAssetViewer(IAssetPropertiesViewer* viewer);

		IOBJECT(AssetsPropertiesViewer);

	protected:
		typedef Vector<IAssetPropertiesViewer*> AssetPropertiesViewersVec;
		typedef Vector<Asset*> AssetsVec;
		typedef Dictionary<const Type*, AssetPropertiesViewersVec> TypeAssetViewersDict;

		UIWidget*                 mHeadWidget = nullptr;         // Asset head widget
		UIImage*                  mAssetIcon = nullptr;          // Asset icon
		UIEditBox*                mAssetNameEdit = nullptr;      // Asset name edit box
		UIButton*                 mRemoveButton = nullptr;       // Remove asset button
		UILabel*                  mAssetPathLabel = nullptr;     // Asset path label

		AssetsVec                 mTargetAssets;                 // Current target assets

		IAssetPropertiesViewer*   mCurrentViewer = nullptr;      // Current asset viewer
		AssetPropertiesViewersVec mAvailableAssetViewers;        // Available asset viewers

	protected:
		// Sets target objects
		void SetTargets(const Vector<IObject*> targets);

		// Tries to get common asset name and inserts to name edit box
		void UpdateCommonName();

		// Tries to get commin assets icon and sets as selected asset icon
		void UpdateCommonIcon();

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

		// Calls when asset name edit box was changed
		void OnNameTextEditChanged(const WString& value);
	};
}