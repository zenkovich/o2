#pragma once

#include "UI/ContextMenu.h"
#include "Utils/Debug/Log/LogStream.h"
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

		// Sets widget as focused
		void FocusWidget(UIWidget* widget);

		// Returns current focused widget
		UIWidget* GetFocusedWidget() const;

		// Sets next widget focused
		void FocusNextWidget();

		// Draws context menus and top drawing widgets
		void Draw();

		// Registering widget for drawing at top of all regular widgets
		void DrawWidgetAtTop(UIWidget* widget);

		// Returns all styles widgets
		const WidgetsVec& GetWidgetStyles() const;

	protected:
		LogStream* mLog = nullptr;           // UI Log stream
		UIWidget*  mFocusedWidget = nullptr; // Current selected widget
		WidgetsVec mFocusableWidgets;        // List of selectable widgets
		WidgetsVec mTopWidgets;              // Top widgets, drawing after mScreenWidget 

		WidgetsVec mStyleSamples;            // Style widgets

	protected:
		// Default constructor
		UIManager();

		// Destructor
		~UIManager();

		// Tries to load style "ui_style.xml"
		void TryLoadStyle();

		friend class Application;
		friend class BaseApplication;
		friend class UICustomDropDown;
		friend class UITree;
		friend class UIWidget;
	};

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
			res = dynamic_cast<_type*>(sample->Clone());
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
