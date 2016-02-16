#pragma once

#include "Assets/AssetInfo.h"
#include "UI/Widget.h"

using namespace o2;

namespace o2
{
	class Text;
}

// ---------------------------
// Draggable asset icon widget
// ---------------------------
class UIAssetIcon: public UIWidget
{
public:
	// Default constructor
	UIAssetIcon();

	// Copy-constructor
	UIAssetIcon(const UIAssetIcon& other);

	// Destructor
	~UIAssetIcon();

	// Copy operator
	UIAssetIcon& operator=(const UIAssetIcon& other);

	// Sets asset info
	void SetAssetInfo(const AssetInfo& info);

	// Returns asset info
	const AssetInfo& GetAssetInfo() const;

	SERIALIZABLE(UIAssetIcon);

protected:
	Text*     mNameText;   // Asset name text layer
	AssetInfo mAssetInfo;  // Asset information
};