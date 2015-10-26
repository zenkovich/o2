#pragma once

#include "Events/CursorEventsListener.h"
#include "UI/Widget.h"

namespace o2
{
	class Sprite;

	// -----------------------
	// Button clickable widget
	// -----------------------
	class Button: public Widget, public CursorEventsListener
	{
	public:
		Property<WString>       caption;     // Caption property. Searches text layer with name "caption" or creates them if he's not exist
		Property<String>        captionc;    // Caption property. Searches text layer with name "caption" or creates them if he's not exist
		Setter<Ptr<ImageAsset>> icon;        // Icon image asset setter. Searches sprite layer with name "icon". Creates him if can't find
		Setter<String>          iconPath;    // Icon image path setter. Searches sprite layer with name "icon". Creates him if can't find
		Setter<AssetId>         iconImageId; // Icon image id setter. Searches sprite layer with name "icon". Creates him if can't find
		Function<void()>        onClick;     // Click event

		// Default constructor
		Button();

		// Copy-constructor
		Button(const Button& other);

		// Assign operator
		Button& operator=(const Button& other);

		// Sets caption of button. Searches text layer with name "caption". If can't find this layer, creates them
		void SetCaption(const WString& text);
		
		// Returns caption text from text layer "caption". Returns no data if layer isn't exist
		WString GetCaption() const;

		// Sets caption of button. Searches text layer with name "caption". If can't find this layer, creates them
		void SetCCaption(const String& text);

		// Returns caption text from text layer "caption". Returns no data if layer isn't exist
		String GetCCaption() const;

		// Sets icon asset. Searches sprite layer "icon". Creates a new icon if isn't exist
		void SetIcon(Ptr<ImageAsset> asset);

		// Sets icon asset path. Searches sprite layer "icon". Creates a new icon if isn't exist
		void SetIcon(const String& imagePath);

		// Sets icon asset id. Searches sprite layer "icon". Creates a new icon if isn't exist
		void SetIcon(AssetId imageAssetId);

		// Returns true if point is in this object
		bool IsUnderPoint(const Vec2F& point);

		// Returns depth (event system will catch listener with highest depth)
		float Depth();

		// Calls when cursor pressed on this. Sets state "pressed" to true
		void OnCursorPressed(const Input::Cursor& cursor);

		// Calls when cursor released (only when cursor pressed this at previous time). Sets state "pressed" to false.
		// Calls onClicked if cursor is still above this
		void OnCursorReleased(const Input::Cursor& cursor);

		// Calls when cursor enters this object. Sets state "select" to true
		void OnCursorEnter(const Input::Cursor& cursor);

		// Calls when cursor exits this object. Sets state "select" to false
		void OnCursorExit(const Input::Cursor& cursor);

		SERIALIZABLE_IMPL(Button);

		IOBJECT(Button)
		{
			BASE_CLASS(Widget);
		}

	protected:
		// Returns "caption" layer or creates them
		Ptr<Text> FindCaptionLayerText();

		// Returns "icon" layer or creates them
		Ptr<Sprite> FindIconLayerSprite();

		// Initializes properties
		void InitializeProperties();
	};
}
