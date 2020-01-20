#pragma once

#include <fstream>
#include "o2/Utils/Types/CommonTypes.h"
#include "o2/Utils/Types/String.h"

#ifdef PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

namespace o2
{
	// ----------
	// Input file
	// ----------
	class InFile
	{
	public:
		// Default constructor
		InFile();

		// Constructor with opening file
		InFile(const String& filename);

		// Destructor
		~InFile();

		// Opening file 
		bool Open(const String& filename);

		// Closing file
		bool Close();

		// Returns file data as string
		String ReadFullData();

		// Read full file data and return size of ridden data
		UInt ReadFullData(void *dataPtr);

		// Read data in dataPtr
		void ReadData(void *dataPtr, UInt bytes);

		// Sets caret position
		void SetCaretPos(UInt pos);

		// Return caret position
		UInt GetCaretPos();

		// Returns full data size
		UInt GetDataSize();

		// Returns true, if file was opened
		bool IsOpened() const;

		// Return file name
		const String& GetFilename() const;

	private:
		std::ifstream mIfstream; // Input stream
		String        mFilename; // File name
		bool          mOpened;   // True, if file was opened

#ifdef PLATFORM_ANDROID
		AAsset* mAsset = nullptr;
#endif
	};

	// -----------
	// Output file
	// -----------
	class OutFile
	{
	public:
		// Default constructor
		OutFile();

		// Constructor with opening file
		OutFile(const String& filename);

		// Destcructor
		~OutFile();

		// Opening file 
		bool Open(const String& filename);

		// Close file
		bool Close();

		// Write some data from dataPtr
		void WriteData(const void* dataPtr, UInt bytes);

		// Returns true, if file was opened
		bool IsOpened() const;

		// Returns file name
		const String& GetFilename() const;

	private:
		std::ofstream mOfstream; // Output stream
		String        mFilename; // File name
		bool          mOpened;   // True, if file was opened
	};
}
