#pragma once

#include "Utils/Property.h"
#include "Utils/Singleton.h"

// User interfaces manager access macros
#define o2UI UIManager::Instance()

namespace o2
{
	class LogStream;
	class UIButton;
	class UICustomDropDown;
	class UICustomList;
	class UIDropDown;
	class UIEditBox;
	class UIHorizontalLayout;
	class UIHorizontalProgress;
	class UIHorizontalScrollBar;
	class UILabel;
	class UIList;
	class UIScrollArea;
	class UIToggle;
	class UIVerticalLayout;
	class UIVerticalProgress;
	class UIVerticalScrollBar;
	class UIWidget;

	// ------------------------------------------------
	// UI manager, contains all root widgets and styles
	// ------------------------------------------------
	class UIManager: public Singleton<UIManager>
	{
	public:
		typedef Vector<Ptr<UIWidget>> WidgetsVec;

	public:
		Accessor<Ptr<UIWidget>, const String&> widget; // Root widget accessor
		
        // Loads widgets style
		void LoadStyle(const String& path);

		// Saves style
		void SaveStyle(const String& path);

		// Clears style widgets
		void ClearStyle();

		// Adds widget style
		void AddWidgetStyle(Ptr<UIWidget> widget, const String& style);

		// Returns widget style by name
		template<typename _type>
		Ptr<_type> GetWidgetStyle(const String& style);

		// Removes widget style
		template<typename _type>
		void RemoveWidgetStyle(const String& style);

		// Adds widget as root
		Ptr<UIWidget> AddWidget();

		// Adds widget as root
		Ptr<UIWidget> AddWidget(Ptr<UIWidget> widget);

		// Adds widget from style
		template<typename _type>
		Ptr<_type> AddWidget(const String& style = "standard");

		// Adds button from style
		Ptr<UIButton> AddButton(const WString& caption, const String& style = "standard");

		// Adds label from style
		Ptr<UILabel> AddLabel(const WString& text, const String& style = "standard");

		// Adds horizontal layout
		Ptr<UIHorizontalLayout> AddHorLayout();

		// Adds vertical layout
		Ptr<UIVerticalLayout> AddVerLayout();

		// Adds horizontal progress bar
		Ptr<UIHorizontalProgress> AddHorProgress(const String& style = "standard");

		// Adds vertical progress bar
		Ptr<UIVerticalProgress> AddVerProgress(const String& style = "standard");

		// Adds horizontal scroll bar
		Ptr<UIHorizontalScrollBar> AddHorScrollBar(const String& style = "standard");

		// Adds vertical scroll bar
		Ptr<UIVerticalScrollBar> AddVerScrollBar(const String& style = "standard");

		// Adds scroll area
		Ptr<UIScrollArea> AddScrollArea(const String& style = "standard");

		// Adds edit box
		Ptr<UIEditBox> AddEditBox(const String& style = "standard");

		// Adds custom list
		Ptr<UICustomList> AddCustomList(const String& style = "standard");

		// Adds text list
		Ptr<UIList> AddList(const String& style = "standard");

		// Adds custom dropdown
		Ptr<UICustomDropDown> AddCustomDropdown(const String& style = "standard");

		// Adds text dropdown
		Ptr<UIDropDown> AddDropdown(const String& style = "standard");

		// Adds toggle
		Ptr<UIToggle> AddToggle(const WString& caption, const String& style = "standard");

		// Creates widget by style
		template<typename _type>
		Ptr<_type> CreateWidget(const String& style = "standard");

		// Creates button by style
		Ptr<UIButton> CreateButton(const WString& caption, const String& style = "standard");

		// Creates label by style
		Ptr<UILabel> CreateLabel(const WString& text, const String& style = "standard");

		// Creates horizontal layout
		Ptr<UIHorizontalLayout> CreateHorLayout();

		// Creates vertical layout
		Ptr<UIVerticalLayout> CreateVerLayout();

		// Creates horizontal progress bar by style
		Ptr<UIHorizontalProgress> CreateHorProgress(const String& style = "standard");

		// CreatesCreates vertical progress bar by style
		Ptr<UIVerticalProgress> CreateVerProgress(const String& style = "standard");

		// Creates horizontal scroll bar by style
		Ptr<UIHorizontalScrollBar> CreateHorScrollBar(const String& style = "standard");

		// Creates vertical scroll bar by style
		Ptr<UIVerticalScrollBar> CreateVerScrollBar(const String& style = "standard");

		// Creates scroll area
		Ptr<UIScrollArea> CreateScrollArea(const String& style = "standard");

		// Creates edit box
		Ptr<UIEditBox> CreateEditBox(const String& style = "standard");

		// Creates list view
		Ptr<UICustomList> CreateCustomList(const String& style = "standard");

		// Creates text list
		Ptr<UIList> CreateList(const String& style = "standard");

		// Creates custom dropdown
		Ptr<UICustomDropDown> CreateCustomDropdown(const String& style = "standard");

		// Creates text dropdown
		Ptr<UIDropDown> CreateDropdown(const String& style = "standard");

		// Creates toggle
		Ptr<UIToggle> CreateToggle(const WString& caption, const String& style = "standard");

		// Removes widget by path
		bool RemoveWidget(const String& path);

		// Removes widget
		bool RemoveWidget(Ptr<UIWidget> widget, bool release = true);

		// Removes all widgets
		void RemoveAllWidgets();

		// Returns widget by path
		Ptr<UIWidget> GetWidget(const String& path);

		// Returns all widgets
		const WidgetsVec& GetAllWidgets() const;

		// Returns main screen widget
		Ptr<UIWidget> GetScreenWidget() const;

		// Sets widget as selected
		void SelectWidget(Ptr<UIWidget> widget);

		// Returns current selected widget
		Ptr<UIWidget> GetSelectedWidget() const;

		// Selects next widget 
		void SelectNextWidget();

	protected:
		Ptr<LogStream> mLog;            // UI Log stream
		Ptr<UIWidget>  mScreenWidget;   // Root screen widget
		Ptr<UIWidget>  mSelectedWidget; // Current selected widget
		WidgetsVec     mTopWidgets;     // Top widgets, drawing after mScreenWidget 

		WidgetsVec     mStyleSamples;   // Style widgets

	protected:
		// Default constructor
		UIManager();

		// Destructor
		~UIManager();

		// Updates widgets
		void Update(float dt);

		// Draws widgets
		void Draw();

		// Updates root widget size
		void UpdateRootSize();

		// Registering top drawing widget
		void RegTopWidget(Ptr<UIWidget> widget);

		// Tries to load style "ui_style.xml"
		void TryLoadStyle();

		// Recursively searches selectable widget
		Ptr<UIWidget> SearchSelectableWidget(Ptr<UIWidget> widget, bool& foundCurrentSelected);

		// Initializes properties
		void InitializeProperties();

		friend class Application;
		friend class BaseApplication;
		friend class UICustomDropDown;

		template<typename _type>
		friend class ITemplPtr;
	};

	template<typename _type>
	Ptr<_type> UIManager::AddWidget(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<_type>(style);
		mScreenWidget->AddChild(res.Cast<UIWidget>());
		return res;
	}

	template<typename _type>
	Ptr<_type> UIManager::GetWidgetStyle(const String& style /*= "standard"*/)
	{
		Ptr<_type> sample;
		for (auto styleWidget : mStyleSamples)
		{
			if (_type::type->ID() == styleWidget->GetTypeId())
			{
				if (style == styleWidget->GetName())
				{
					sample = styleWidget.Cast<_type>();
					break;
				}
				else if (!sample)
					sample = styleWidget.Cast<_type>();
			}
		}

		return sample;
	}

	template<typename _type>
	void UIManager::RemoveWidgetStyle(const String& style)
	{
		Ptr<_type> widgetStyle = GetWidgetStyle<_type>(style);
		if (widgetStyle)
		{
			mStyleSamples.Remove(widgetStyle);
			widgetStyle.Release();
		}
	}

	template<typename _type>
	Ptr<_type> UIManager::CreateWidget(const String& style /*= "standard"*/)
	{
		Ptr<_type> sample = GetWidgetStyle<_type>(style);
		Ptr<_type> res;

		if (sample)
		{
			res = (_type*)sample->Clone();
		}
		else
		{
			mLog->Warning("Can't find style %s for %s", style, _type::type->Name());
			res = mnew _type();
		}

		return res;
	}

}