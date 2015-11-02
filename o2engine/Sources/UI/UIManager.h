#pragma once

#include "Utils/Property.h"
#include "Utils/Singleton.h"

// User interfaces manager access macros
#define o2UI UIManager::Instance()

namespace o2
{
	class Widget;
	class Button;
	class Label;
	class LogStream;

	// ------------------------------------------------
	// UI manager, contains all root widgets and styles
	// ------------------------------------------------
	class UIManager: public Singleton<UIManager>
	{
	public:
		typedef Vector<Ptr<Widget>> WidgetsVec;

	public:
		Accessor<Ptr<Widget>, const String&> widget; // Root widget accessor

		// Adds widget as root
		Ptr<Widget> AddWidget();

		// Adds widget as root
		Ptr<Widget> AddWidget(Ptr<Widget> widget);

		// Adds widget from style
		template<typename _type>
		Ptr<_type> AddWidget(const String& style = "standard");

		// Adds button from style
		Ptr<Button> AddButton(const String& caption, const String& style = "standard");

		// Adds label from style
		Ptr<Label> AddLabel(const String& text, const String& style = "standard");

		// Removes widget by path
		bool RemoveWidget(const String& path);

		// Removes widget
		bool RemoveWidget(Ptr<Widget> widget, bool release = true);

		// Returns widget by path
		Ptr<Widget> GetWidget(const String& path);

		// Removes all widgets
		void RemoveAllWidgets();

		// Returns all widgets
		const WidgetsVec& GetAllWidgets() const;

		// Loads widgets style
		void LoadStyle(const String& path);

		// Saves style
		void SaveStyle(const String& path);

		// Adds widget style
		void AddStyle(Ptr<Widget> widget, const String& style);

		// Returns widget style by name
		Ptr<Widget> GetWidgetStyle(const String& style);

		// Creates widget by style
		template<typename _type>
		Ptr<_type> CreateWidget(const String& style = "standard");

		// Creates button by style
		Ptr<Button> CreateButton(const String& caption, const String& style = "standard");

		// Creates label by style
		Ptr<Label> CreateLabel(const String& text, const String& style = "standard");

	protected:
		Ptr<Widget>    mScreenWidget; // Root screen widget
		WidgetsVec     mStyleSamples; // Style widgets
		Ptr<LogStream> mLog;          // UI Log stream

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

		// Initializes properties
		void InitializeProperties();

		friend class IApplication;
		friend class BaseApplication;

		template<typename _type>
		friend class ITemplPtr;
	};

	template<typename _type>
	Ptr<_type> UIManager::AddWidget(const String& style /*= "standard"*/)
	{
		auto res = CreateWidget<_type>(style);
		mScreenWidget->AddChild(res.Cast<Widget>());
		return res;
	}

	template<typename _type>
	Ptr<_type> UIManager::CreateWidget(const String& style /*= "standard"*/)
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