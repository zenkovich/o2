#pragma once

#include "UI/ContextMenu.h"
#include "Utils/Log/LogStream.h"
#include "Utils/Property.h"
#include "Utils/Reflection/Type.h"
#include "Utils/Singleton.h"

// User interfaces manager access macros
#define o2UI UIManager::Instance()

namespace o2
{
	class UIButton;
	class UICustomDropDown;
	class UICustomList;
	class UIDropDown;
	class UIEditBox;
	class UIHorizontalLayout;
	class UIHorizontalProgress;
	class UIHorizontalScrollBar;
	class UIImage;
	class UILabel;
	class UIList;
	class UIScrollArea;
	class UIToggle;
	class UIVerticalLayout;
	class UIVerticalProgress;
	class UIVerticalScrollBar;
	class UIWidget;
	class UIWindow;

#undef CreateWindow

	// ------------------------------------------------
	// UI manager, contains all root widgets and styles
	// ------------------------------------------------
	class UIManager: public Singleton<UIManager>
	{
	public:
		typedef Vector<UIWidget*> WidgetsVec;

	public:
		Accessor<UIWidget*, const String&> widget; // Root widget accessor

		// Loads widgets style
		void LoadStyle(const String& path);

		// Saves style
		void SaveStyle(const String& path);

		// Clears style widgets
		void ClearStyle();

		// Adds widget style
		void AddWidgetStyle(UIWidget* widget, const String& style);

		// Returns widget style by name
		template<typename _type>
		_type* GetWidgetStyle(const String& style);

		// Removes widget style
		template<typename _type>
		void RemoveWidgetStyle(const String& style);

		// Adds widget as root
		UIWidget* AddWidget();

		// Adds widget as root
		UIWidget* AddWidget(UIWidget* widget);

		// Adds widget from style
		template<typename _type>
		_type* AddWidget(const String& style = "standard");

		// Adds button from style
		UIButton* AddButton(const WString& caption, const String& style = "standard");

		// Adds button from style
		UIButton* AddButton(const WString& caption, const Function<void()>& onClick = Function<void()>(),
							const String& style = "standard");

		// Adds window from style
		UIWindow* AddWindow(const WString& caption, const String& style = "standard");

		// Adds label from style
		UILabel* AddLabel(const WString& text, const String& style = "standard");

		// Adds horizontal layout
		UIHorizontalLayout* AddHorLayout();

		// Adds vertical layout
		UIVerticalLayout* AddVerLayout();

		// Adds horizontal progress bar
		UIHorizontalProgress* AddHorProgress(const String& style = "standard");

		// Adds vertical progress bar
		UIVerticalProgress* AddVerProgress(const String& style = "standard");

		// Adds horizontal scroll bar
		UIHorizontalScrollBar* AddHorScrollBar(const String& style = "standard");

		// Adds vertical scroll bar
		UIVerticalScrollBar* AddVerScrollBar(const String& style = "standard");

		// Adds scroll area
		UIScrollArea* AddScrollArea(const String& style = "standard");

		// Adds edit box
		UIEditBox* AddEditBox(const String& style = "standard");

		// Adds custom list
		UICustomList* AddCustomList(const String& style = "standard");

		// Adds text list
		UIList* AddList(const String& style = "standard");

		// Adds custom dropdown
		UICustomDropDown* AddCustomDropdown(const String& style = "standard");

		// Adds text dropdown
		UIDropDown* AddDropdown(const String& style = "standard");

		// Adds toggle
		UIToggle* AddToggle(const WString& caption, const String& style = "standard");

		// Adds image
		UIImage* AddImage(const String& name);

		// Creates widget by style
		template<typename _type>
		_type* CreateWidget(const String& style = "standard");

		// Creates button by style
		UIButton* CreateButton(const WString& caption, const Function<void()>& onClick = Function<void()>(),
								const String& style = "standard");

		// Creates button by style
		UIWindow* CreateWindow(const WString& caption, const String& style = "standard");

		// Creates label by style
		UILabel* CreateLabel(const WString& text, const String& style = "standard");

		// Creates horizontal layout
		UIHorizontalLayout* CreateHorLayout();

		// Creates vertical layout
		UIVerticalLayout* CreateVerLayout();

		// Creates horizontal progress bar by style
		UIHorizontalProgress* CreateHorProgress(const String& style = "standard");

		// CreatesCreates vertical progress bar by style
		UIVerticalProgress* CreateVerProgress(const String& style = "standard");

		// Creates horizontal scroll bar by style
		UIHorizontalScrollBar* CreateHorScrollBar(const String& style = "standard");

		// Creates vertical scroll bar by style
		UIVerticalScrollBar* CreateVerScrollBar(const String& style = "standard");

		// Creates scroll area
		UIScrollArea* CreateScrollArea(const String& style = "standard");

		// Creates edit box
		UIEditBox* CreateEditBox(const String& style = "standard");

		// Creates list view
		UICustomList* CreateCustomList(const String& style = "standard");

		// Creates text list
		UIList* CreateList(const String& style = "standard");

		// Creates custom dropdown
		UICustomDropDown* CreateCustomDropdown(const String& style = "standard");

		// Creates text dropdown
		UIDropDown* CreateDropdown(const String& style = "standard");

		// Creates toggle
		UIToggle* CreateToggle(const WString& caption, const String& style = "standard");

		// Creates image
		UIImage* CreateImage(const String& name);

		// Removes widget by path
		bool RemoveWidget(const String& path);

		// Removes widget
		bool RemoveWidget(UIWidget* widget, bool release = true);

		// Removes all widgets
		void RemoveAllWidgets();

		// Returns widget by path
		UIWidget* GetWidget(const String& path);

		// Returns all widgets
		const WidgetsVec& GetAllWidgets() const;

		// Returns main screen widget
		UIWidget* GetScreenWidget() const;

		// Sets widget as focused
		void FocusWidget(UIWidget* widget);

		// Returns current focused widget
		UIWidget* GetFocusedWidget() const;

		// Sets next widget focused
		void FocusNextWidget();

		// Updates widgets
		void Update(float dt);

		// Draws widgets
		void Draw();

		// Registering widget for draing at top of all regular widgets
		void DrawWidgetAtTop(UIWidget* widget);

	protected:
		LogStream* mLog;            // UI Log stream
		UIWidget*  mScreenWidget;   // Root screen widget
		UIWidget*  mSelectedWidget; // Current selected widget
		WidgetsVec mTopWidgets;     // Top widgets, drawing after mScreenWidget 

		WidgetsVec mStyleSamples;   // Style widgets

	protected:
		// Default constructor
		UIManager();

		// Destructor
		~UIManager();

		// Updates root widget size
		void UpdateRootSize();

		// Tries to load style "ui_style.xml"
		void TryLoadStyle();

		// Recursively searches selectable widget
		UIWidget* SearchSelectableWidget(UIWidget* widget, bool& foundCurrentSelected);

		// Initializes properties
		void InitializeProperties();

		friend class Application;
		friend class BaseApplication;
		friend class UICustomDropDown;
		friend class UITree;

		template<typename _type>
		friend class ITemplPtr;
	};

	template<typename _type>
	_type* UIManager::AddWidget(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<_type>(style);
		mScreenWidget->AddChild((UIWidget*)res);
		return res;
	}

	template<typename _type>
	_type* UIManager::GetWidgetStyle(const String& style /*= "standard"*/)
	{
		_type* sample = nullptr;
		for (auto styleWidget : mStyleSamples)
		{
			if (TypeOf(_type)== styleWidget->GetType())
			{
				if (style == styleWidget->GetName())
				{
					sample = (_type*)styleWidget;
					break;
				}
				else if (!sample)
					sample = (_type*)styleWidget;
			}
		}

		return sample;
	}

	template<typename _type>
	void UIManager::RemoveWidgetStyle(const String& style)
	{
		_type* widgetStyle = GetWidgetStyle<_type>(style);
		if (widgetStyle)
		{
			mStyleSamples.Remove(widgetStyle);
			delete widgetStyle;
		}
	}

	template<typename _type>
	_type* UIManager::CreateWidget(const String& style /*= "standard"*/)
	{
		_type* sample = GetWidgetStyle<_type>(style);
		_type* res;

		if (sample)
		{
			res = (_type*)sample->Clone();
		}
		else
		{
			mLog->Warning("Can't find style %s for %s", style, TypeOf(_type).GetName());
			res = mnew _type();
		}

		if (TypeOf(_type) != TypeOf(UIContextMenu))
			res->SetVisibleForcible(true);

		return res;
	}

}