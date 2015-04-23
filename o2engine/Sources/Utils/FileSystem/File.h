#pragma once

#include <fstream>
#include "Utils/String.h"
#include "Utils/CommonTypes.h"

namespace o2
{
	/** Input file. */
	class InFile
	{
		std::ifstream mIfstream; /**< Input stream. */
		TString        mFilename; /**< File name. */
		bool          mOpened;   /**< True, if file was opened. */

	public:
		InFile();
		InFile(const TString& filename);
		~InFile();

		/** Opening file with specified extension. If type is cFileType::FT_FILE, file will opened wothout adding extension,
		* else file name will be combine with extension string from file system. */
		bool Open(const TString& filename);

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
		const TString& GetFilename() const;
	};

	/** Output file. */
	class OutFile
	{
		std::ofstream mOfstream; /**< Output stream. */
		TString        mFilename; /**< File name. */
		bool          mOpened;   /**< True, if file was opened. */

	public:
		OutFile();
		OutFile(const TString& filename);
		~OutFile();

		/** Opening file with specified extension. If type is cFileType::FT_FILE, file will opened wothout adding extension,
		* else file name will be combine with first extension string from file system. */
		bool Open(const TString& filename);

		/** Close file. */
		bool Close();

		/** Write some data from dataPtr. */
		void WriteData(const void* dataPtr, UInt bytes);

		/** Returns true, if file was opened. */
		bool IsOpened() const;

		/** Returns file name. */
		const TString& GetFilename() const;
	};
}