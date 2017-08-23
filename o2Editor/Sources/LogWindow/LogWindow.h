#pragma once

#include "Core/WindowsSystem/IEditorWindow.h"
#include "Utils/Log/LogStream.h"

using namespace o2;

namespace o2
{
	class UILabel;
	class UILongList;
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
		void Update(float dt);

	protected:
		UILongList* mList;
		UIWidget*   mLastMessageView;
		Text*       mMessagesCountLabel;
		Text*       mWarningsCountLabel;
		Text*       mErrorsCountLabel;

		MessagesVec mAllMessages;
		MessagesVec mVisibleMessages;

		bool        mRegularMessagesEnabled;
		bool        mWarningMessagesEnabled;
		bool        mErrorMessagesEnabled;

		int         mRegularMessagesCount;
		int         mWarningMessagesCount;
		int         mErrorMessagesCount;

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
		void SetupListMessage(UIWidget* item, UnknownType* object);

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
