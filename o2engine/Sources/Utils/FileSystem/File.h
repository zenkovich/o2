#pragma once

#include <fstream>
#include "Utils/String.h"
#include "Utils/CommonTypes.h"

namespace o2
{
	enum class FileType { File, Image, Config, Atlas };

	/** Input file. */
	class InFile
	{
		std::ifstream mIfstream; /**< Input stream. */
		String        mFilename; /**< File name. */
		bool          mOpened;   /**< True, if file was opened. */

	public:
		InFile();
		InFile(const String& filename);
		~InFile();

		/** Opening file with specified extension. If type is cFileType::FT_FILE, file will opened wothout adding extension,
		* else file name will be combine with extension string from file system. */
		bool Open(const String& filename);

		/** Closing file. */
		bool Close();

		/** Read full file data and return size of readed data. */
		UInt ReadFullData(void *dataPtr);

		/** Read data in dataPtr. */
		void ReadData(void *dataPtr, UInt bytes);

		/** Sets caret position. */
		void SetCaretPos(UInt pos);

		/** Return caret position. */
		UInt GetCaretPos();

		/** Returns full data size. */
		UInt GetDataSize();

		/** Returns true, if file was opened. */
		bool IsOpened() const;

		/** Return file name. */
		const String& GetFilename() const;
	};

	/** Output file. */
	class OutFile
	{
		std::ofstream mOfstream; /**< Output stream. */
		String        mFilename; /**< File name. */
		bool          mOpened;   /**< True, if file was opened. */

	public:
		OutFile();
		OutFile(const String& filename);
		~OutFile();

		/** Opening file with specified extension. If type is cFileType::FT_FILE, file will opened wothout adding extension,
		* else file name will be combine with first extension string from file system. */
		bool Open(const String& filename);

		/** Close file. */
		bool Close();

		/** Write some data from dataPtr. */
		void WriteData(const void* dataPtr, UInt bytes);

		/** Returns true, if file was opened. */
		bool IsOpened() const;

		/** Returns file name. */
		const String& GetFilename() const;
	};
}