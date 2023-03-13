#pragma once

#include "o2/Utils/Debug/Log/LogStream.h"
#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"

using namespace o2;

namespace o2
{
	class Label;
	class LongList;
	class Text;
}

namespace Editor
{
	// write comment what LogWindow class does

	class LogWindow: public IEditorWindow, public LogStream
	{
		IOBJECT(LogWindow);

	public:
		// -----------
		// Log message
		// -----------
		class LogMessage
		{
		public:
			// Message type
			enum class Type { Regular, Warning, Error };

			Type   type;    // Message type
			String message; // Message text
			int    idx;	    // Message index

			// Check equals operator
			bool operator==(const LogMessage& other) const;
		};

		// Updates window logic
		void Update(float dt) override;

	protected:
		LongList* mList = nullptr;               // List of messages
		Widget*   mLastMessageView = nullptr;    // Last message view
		Text*     mMessagesCountLabel = nullptr; // Messages count label
		Text*     mWarningsCountLabel = nullptr; // Warnings count label
		Text*     mErrorsCountLabel = nullptr;   // Errors count label

		Vector<LogMessage> mAllMessages;     // All messages
		Vector<LogMessage> mVisibleMessages; // Visible messages

		bool mRegularMessagesEnabled = true; // Is regular messages enabled
		bool mWarningMessagesEnabled = true; // Is warning messages enabled
		bool mErrorMessagesEnabled = true;   // Is error messages enabled

		int mRegularMessagesCount = 0; // Regular messages count
		int mWarningMessagesCount = 0; // Warning messages count
		int mErrorMessagesCount = 0;   // Error messages count

	public:
		// Default constructor
		LogWindow();

		// Destructor
		~LogWindow();

	protected:
		// Initializes window
		void InitializeWindow();

		// Called when clear button pressed
		void OnClearPressed();

		// Called when regular messages toggled
		void OnRegularMessagesToggled(bool value);

		// Called when warning messages toggled
		void OnWarningMessagesToggled(bool value);

		// Called when error messages toggled
		void OnErrorMessagesToggled(bool value);

		// Updates visible messages
		void UpdateVisibleMessages();

		// Returns visible items count
		int GetVisibleMessagesCount();

		// Returns array of visible messages in range from min to max
		Vector<void*> GetVisibleMessagesRange(int min, int max);

		// Sets list item by message
		void SetupListMessage(Widget* item, void* object);

		// Outs string to stream
		void OutStrEx(const WString& str) override;

		// Outs error to stream
		void OutErrorEx(const WString& str) override;

		// Outs warning to stream
		void OutWarningEx(const WString& str) override;

		// Updates last message view
		void UpdateLastMessageView();
	};
}

PRE_ENUM_META(Editor::LogWindow::LogMessage::Type);

CLASS_BASES_META(Editor::LogWindow)
{
	BASE_CLASS(Editor::IEditorWindow);
	BASE_CLASS(o2::LogStream);
}
END_META;
CLASS_FIELDS_META(Editor::LogWindow)
{
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mList);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mLastMessageView);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mMessagesCountLabel);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mWarningsCountLabel);
	FIELD().PROTECTED().DEFAULT_VALUE(nullptr).NAME(mErrorsCountLabel);
	FIELD().PROTECTED().NAME(mAllMessages);
	FIELD().PROTECTED().NAME(mVisibleMessages);
	FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mRegularMessagesEnabled);
	FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mWarningMessagesEnabled);
	FIELD().PROTECTED().DEFAULT_VALUE(true).NAME(mErrorMessagesEnabled);
	FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mRegularMessagesCount);
	FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mWarningMessagesCount);
	FIELD().PROTECTED().DEFAULT_VALUE(0).NAME(mErrorMessagesCount);
}
END_META;
CLASS_METHODS_META(Editor::LogWindow)
{

	FUNCTION().PUBLIC().SIGNATURE(void, Update, float);
	FUNCTION().PUBLIC().CONSTRUCTOR();
	FUNCTION().PROTECTED().SIGNATURE(void, InitializeWindow);
	FUNCTION().PROTECTED().SIGNATURE(void, OnClearPressed);
	FUNCTION().PROTECTED().SIGNATURE(void, OnRegularMessagesToggled, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnWarningMessagesToggled, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, OnErrorMessagesToggled, bool);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateVisibleMessages);
	FUNCTION().PROTECTED().SIGNATURE(int, GetVisibleMessagesCount);
	FUNCTION().PROTECTED().SIGNATURE(Vector<void*>, GetVisibleMessagesRange, int, int);
	FUNCTION().PROTECTED().SIGNATURE(void, SetupListMessage, Widget*, void*);
	FUNCTION().PROTECTED().SIGNATURE(void, OutStrEx, const WString&);
	FUNCTION().PROTECTED().SIGNATURE(void, OutErrorEx, const WString&);
	FUNCTION().PROTECTED().SIGNATURE(void, OutWarningEx, const WString&);
	FUNCTION().PROTECTED().SIGNATURE(void, UpdateLastMessageView);
}
END_META;
