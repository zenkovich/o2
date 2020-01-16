#pragma once

#include "o2/Scene/UI/WidgetLayout.h"
#include "o2/Utils/Math/Vector2.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Singleton.h"
#include "o2Editor/Core/WindowsSystem/WindowsLayout.h"

using namespace o2;

namespace o2
{
	class Widget;
}

// Editor configuration access macros
#define o2EditorConfig EditorConfig::Instance()

namespace Editor
{
	class DockWindowPlace;

	// -------------------------
	// Application configuration
	// -------------------------
	class EditorConfig: public ISerializable, public Singleton<EditorConfig>
	{
	public:
		// Default constructor. Loads data and applies to application 
		EditorConfig();

		// Destructor. Saves application configuration
		~EditorConfig();

		// Returns user data from global config
		DataNode& GetGlobalUserData();

		// Returns user data from project config
		DataNode& GetProjectUserData();

		SERIALIZABLE(EditorConfig);

	public:
		class GlobalConfig: public ISerializable
		{
		public:
			String         mLastOpenedProjectpath; // Last opened project path @SERIALIZABLE
			WindowsLayout  mDefaultLayout;         // Default windows layout, using in resetting @SERIALIZABLE
			DataNode       mUserData;              // User data  @SERIALIZABLE

			Map<String, WindowsLayout> mAvailableLayouts;      // Available windows layouts @SERIALIZABLE

			SERIALIZABLE(GlobalConfig);
		};

		class ProjectConfig: public ISerializable
		{
		public:
			Vec2I         mWindowSize = Vec2I(800, 600); // Application window size @SERIALIZABLE
			Vec2I         mWindowPosition;               // Application window position @SERIALIZABLE
			bool          mMaximized = true;             // Is application window is maximized @SERIALIZABLE
			WindowsLayout mLayout;                       // Windows layout @SERIALIZABLE
			DataNode      mUserData;                     // User data  @SERIALIZABLE

			SERIALIZABLE(ProjectConfig);
		};
	protected:
		String mConfigPath = "EditorConfig.xml";
		String mGlobalConfigPath = "Config.xml";

		bool          mConfigsLoaded = false; // True if configurations were loaded
		ProjectConfig mProjectConfig;         // Project editor config
		GlobalConfig  mGlobalConfig;          // Global editor config for all projects

	protected:
		// Saves global configs
		void SaveGlobalConfigs();

		// Saves project configs
		void SaveProjectConfigs();

		// Loads and applies configs
		void LoadConfigs();

		// Loads project configs
		void LoadProjectConfig();

		// Loads global configs
		void LoadGlobalConfig();

		// Updates window configs
		void OnWindowChange();

		friend class EditorApplication;
		friend class MenuPanel;
		friend class WindowsManager;
	};
}

CLASS_BASES_META(Editor::EditorConfig)
{
	BASE_CLASS(o2::ISerializable);
	BASE_CLASS(o2::Singleton<EditorConfig>);
}
END_META;
CLASS_FIELDS_META(Editor::EditorConfig)
{
	PROTECTED_FIELD(mConfigPath);
	PROTECTED_FIELD(mGlobalConfigPath);
	PROTECTED_FIELD(mConfigsLoaded);
	PROTECTED_FIELD(mProjectConfig);
	PROTECTED_FIELD(mGlobalConfig);
}
END_META;
CLASS_METHODS_META(Editor::EditorConfig)
{

	PUBLIC_FUNCTION(DataNode&, GetGlobalUserData);
	PUBLIC_FUNCTION(DataNode&, GetProjectUserData);
	PROTECTED_FUNCTION(void, SaveGlobalConfigs);
	PROTECTED_FUNCTION(void, SaveProjectConfigs);
	PROTECTED_FUNCTION(void, LoadConfigs);
	PROTECTED_FUNCTION(void, LoadProjectConfig);
	PROTECTED_FUNCTION(void, LoadGlobalConfig);
	PROTECTED_FUNCTION(void, OnWindowChange);
}
END_META;

CLASS_BASES_META(Editor::EditorConfig::GlobalConfig)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::EditorConfig::GlobalConfig)
{
	PUBLIC_FIELD(mLastOpenedProjectpath).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mDefaultLayout).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mUserData).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mAvailableLayouts).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(Editor::EditorConfig::GlobalConfig)
{
}
END_META;

CLASS_BASES_META(Editor::EditorConfig::ProjectConfig)
{
	BASE_CLASS(o2::ISerializable);
}
END_META;
CLASS_FIELDS_META(Editor::EditorConfig::ProjectConfig)
{
	PUBLIC_FIELD(mWindowSize).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mWindowPosition).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mMaximized).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mLayout).SERIALIZABLE_ATTRIBUTE();
	PUBLIC_FIELD(mUserData).SERIALIZABLE_ATTRIBUTE();
}
END_META;
CLASS_METHODS_META(Editor::EditorConfig::ProjectConfig)
{
}
END_META;
