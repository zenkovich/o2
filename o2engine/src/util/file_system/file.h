#ifndef FILE_H
#define FILE_H

#include <fstream>

#include "public.h"
OPEN_O2_NAMESPACE

struct cFileType
{
	enum value { FT_FILE = 0, FT_IMAGE, FT_CONFIG };
};

/** Input file. */
class cInFile
{
	std::ifstream mIfstream; /**< Input stream. */
	std::string   mFilename; /**< File name. */
	bool          mOpened;   /**< True, if file was opened. */

public:
	cInFile();
	cInFile(const std::string& filename, cFileType::value type = cFileType::FT_FILE);
	~cInFile();

	/** Opening file with specified extension. If type is cFileType::FT_FILE, file will opened wothout adding extension,
	  * else file name will be combine with extension string from file system. */
	bool open(const std::string& filename, cFileType::value type = cFileType::FT_FILE);

	/** Closing file. */
	bool close();
		               
	/** Read full file data and return size of readed data. */
	uint32 readFullData(void *dataPtr);

	/** Read data in dataPtr. */
	void readData(void *dataPtr, uint32 bytes);
		               
	/** Sets caret position. */
	void setCaretPos(uint32 pos);

	/** Return caret position. */
	uint32 getCaretPos();
		              
	/** Returns full data size. */
	uint32 getDataSize();
		               
	/** Returns true, if file was opened. */
	bool isOpened() const;

	/** Return file name. */
	const std::string& getFilename() const;
};

/** Output file. */
class cOutFile
{
	std::ofstream mOfstream; /**< Output stream. */
	std::string   mFilename; /**< File name. */
	bool          mOpened;   /**< True, if file was opened. */

public:
	cOutFile();
	cOutFile(const std::string& filename, cFileType::value type = cFileType::FT_FILE);
	~cOutFile();

	/** Opening file with specified extension. If type is cFileType::FT_FILE, file will opened wothout adding extension,
	  * else file name will be combine with first extension string from file system. */
	bool open(const std::string& filename, cFileType::value type = cFileType::FT_FILE);

	/** Close file. */
	bool close();
		   
	/** Write some data from dataPtr. */
	void writeData(const void* dataPtr, uint32 bytes);
		               
	/** Returns true, if file was opened. */
	bool isOpened() const;

	/** Returns file name. */
	const std::string& getFilename() const;
};

CLOSE_O2_NAMESPACE

#endif //FILE_H