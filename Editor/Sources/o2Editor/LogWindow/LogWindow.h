#pragma once

#include "o2Editor/Core/WindowsSystem/IEditorWindow.h"
#include "o2/Utils/Debug/Log/LogStream.h"

using namespace o2;

namespace o2
{
	class Label;
	class LongList;
	class Text;
}

namespace Editor
{
	class LogWindow: public IEditorWindow, public LogStream
	{
		IOBJECT(LogWindow);

	public:
		class LogMessage
		{
		public:
			enum class Type { Regular, Warning, Error };

			Type   type;
			String message;
			int    idx;

			bool operator==(const LogMessage& other) const;
		};
		typedef Vector<LogMessage> MessagesVec;

		// Updates window logic
		void Update(float dt) override;

	protected:
		LongList* mList = nullptr;
		Widget*   mLastMessageView = nullptr;
		Text*     mMessagesCountLabel = nullptr;
		Text*     mWarningsCountLabel = nullptr;
		Text*     mErrorsCountLabel = nullptr;

		MessagesVec mAllMessages;
		MessagesVec mVisibleMessages;

		bool mRegularMessagesEnabled;
		bool mWarningMessagesEnabled;
		bool mErrorMessagesEnabled;

		int mRegularMessagesCount;
		int mWarningMessagesCount;
		int mErrorMessagesCount;

	public:
		// Default constructor
		LogWindow();

		// Destructor
		~LogWindow();

	protected:
		// Initializes window
		void InitializeWindow();

		// It is called when clear button pressed
		void OnClearPressed();

		// It is called when regular messages toggled
		void OnRegularMessagesToggled(bool value);

		// It is called when warning messages toggled
		void OnWarningMessagesToggled(bool value);

		// It is called when error messages toggled
		void OnErrorMessagesToggled(bool value);

		// Updates visible messages
		void UpdateVisibleMessages();

		// Returns visible items count
		int GetVisibleMessagesCount();

		// Returns array of visible messages in range from min to max
		Vector<UnknownType*> GetVisibleMessagesRange(int min, int max);

		// Sets list item by message
		void SetupListMessage(Widget* item, UnknownType* object);

		// Outs string to stream
		void OutStrEx(const WString& str);

		// Outs error to stream
		void OutErrorEx(const WString& str);

		// Outs warning to stream
		void OutWarningEx(const WString& str);

		// Updates last message view
		void UpdateLastMessageView();
	};
}

CLASS_BASES_META(Editor::LogWindow)
{
	BASE_CLASS(Editor::IEditorWindow);
	BASE_CLASS(o2::LogStream);
}
END_META;
CLASS_FIELDS_META(Editor::LogWindow)
{
	PROTECTED_FIELD(mList);
	PROTECTED_FIELD(mLastMessageView);
	PROTECTED_FIELD(mMessagesCountLabel);
	PROTECTED_FIELD(mWarningsCountLabel);
	PROTECTED_FIELD(mErrorsCountLabel);
	PROTECTED_FIELD(mAllMessages);
	PROTECTED_FIELD(mVisibleMessages);
	PROTECTED_FIELD(mRegularMessagesEnabled);
	PROTECTED_FIELD(mWarningMessagesEnabled);
	PROTECTED_FIELD(mErrorMessagesEnabled);
	PROTECTED_FIELD(mRegularMessagesCount);
	PROTECTED_FIELD(mWarningMessagesCount);
	PROTECTED_FIELD(mErrorMessagesCount);
}
END_META;
CLASS_METHODS_META(Editor::LogWindow)
{

	PUBLIC_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, InitializeWindow);
	PROTECTED_FUNCTION(void, OnClearPressed);
	PROTECTED_FUNCTION(void, OnRegularMessagesToggled, bool);
	PROTECTED_FUNCTION(void, OnWarningMessagesToggled, bool);
	PROTECTED_FUNCTION(void, OnErrorMessagesToggled, bool);
	PROTECTED_FUNCTION(void, UpdateVisibleMessages);
	PROTECTED_FUNCTION(int, GetVisibleMessagesCount);
	PROTECTED_FUNCTION(Vector<UnknownType*>, GetVisibleMessagesRange, int, int);
	PROTECTED_FUNCTION(void, SetupListMessage, Widget*, UnknownType*);
	PROTECTED_FUNCTION(void, OutStrEx, const WString&);
	PROTECTED_FUNCTION(void, OutErrorEx, const WString&);
	PROTECTED_FUNCTION(void, OutWarningEx, const WString&);
	PROTECTED_FUNCTION(void, UpdateLastMessageView);
}
END_META;
