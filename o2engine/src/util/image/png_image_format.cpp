#include "png_image_format.h"

#include "bitmap.h"
#include "libpng/png.h"
#include "util/file_system/file.h"
#include "util/log.h"

OPEN_O2_NAMESPACE

void CustomPngReadFn(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead)
{
	void* io_ptr = png_get_io_ptr(png_ptr);
	if (io_ptr == NULL) return;

	InFile* file = (InFile*)io_ptr;

	file->ReadData(outBytes, byteCountToRead);
}

void CustomPngWriteFn(png_structp png_ptr, png_bytep bytes, png_size_t byteCountToWrite)
{
	void* io_ptr = png_get_io_ptr(png_ptr);
	if (io_ptr == NULL) return;

	OutFile* file = (OutFile*)io_ptr;

	file->WriteData(bytes, byteCountToWrite);
}

void CustomPngFlushFn(png_structp png_ptr) {}

bool LoadPngImage(const String& fileName, Bitmap* image, bool errors /*= true*/, LogStream* plog /*= NULL*/)
{
	LogStream* log = plog ? plog:gLog;

	InFile pngImageFile(fileName, FileType::Image);
	if (!pngImageFile.IsOpened())
	{
		if (errors) log->Error("Can't load PNG file '%s'\n", fileName.c_str());
		return false;
	}

	//header for testing if it is a png
	png_byte header[8];

	//read the header
	pngImageFile.ReadData(header, 8);

	//test if png
	int is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png)
	{
		if (errors) log->Error("Can't load PNG file '%s': not PNG\n", fileName.c_str());
		return false;
	}

	//create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		if (errors) log->Error("Can't load PNG file '%s': TEXTURE_LOAD_ERROR\n", fileName.c_str());
		return false;
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);

		if (errors) log->Error("Can't load PNG file '%s': TEXTURE_LOAD_ERROR\n", fileName.c_str());
		return false;
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

		if (errors) log->Error("Can't load PNG file '%s': TEXTURE_LOAD_ERROR\n", fileName.c_str());
		return false;
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

		if (errors) log->Error("Can't load PNG file '%s': TEXTURE_LOAD_ERROR\n", fileName.c_str());
		return false;
	}

	//init png reading
	png_set_read_fn(png_ptr, &pngImageFile, CustomPngReadFn);

	//png_init_io(png_ptr, fp);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 twidth, theight;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// Allocate the image_data as a big block, to be given to opengl
	image->Create(Bitmap::Format::R8G8B8A8, Vec2I(twidth, theight));
	png_byte *image_data = image->GetData();
	if (!image_data)
	{
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

		if (errors) log->Error("Can't load PNG file '%s': TEXTURE_LOAD_ERROR\n", fileName.c_str());
		return false;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = mnew png_bytep[theight];
	if (!row_pointers)
	{
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		SafeReleaseArr(image_data);

		if (errors) log->Error("Can't load PNG file '%s': TEXTURE_LOAD_ERROR\n", fileName.c_str());
		return false;
	}

	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < (int)theight; ++i)
		row_pointers[theight - 1 - i] = image_data + i * rowbytes;

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	SafeReleaseArr(row_pointers);

	return true;
}

bool SavePngImage(const String& fileName, const Bitmap* image, LogStream* plog /*= NULL*/)
{
	LogStream* log = plog ? plog:gLog;

	OutFile pngImageFile(fileName);
	if (!pngImageFile.IsOpened())
	{
		log->Error("Can't save PNG file '%s'\n", fileName.c_str());
		return false;
	}

	png_structp png_ptr;
	png_infop info_ptr;

	/* initialize stuff */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
	{
		log->Error("Can't save PNG file '%s': png_create_write_struct failed\n", fileName.c_str());
		return false;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		log->Error("Can't save PNG file '%s': png_create_info_struct failed\n", fileName.c_str());
		return false;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		log->Error("Can't save PNG file '%s': Error during init_io\n", fileName.c_str());
		return false;
	}

	//png_init_io(png_ptr, fp);

	png_set_write_fn(png_ptr, &pngImageFile, CustomPngWriteFn, CustomPngFlushFn);

	/* write header */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		log->Error("Can't save PNG file '%s': Error during writing header\n", fileName.c_str());
		return false;
	}

	png_byte bit_depth = 8, color_type = PNG_COLOR_TYPE_RGB_ALPHA;

	png_set_IHDR(png_ptr, info_ptr, (unsigned int)image->GetSize().x, (unsigned int)image->GetSize().y,
				 bit_depth, color_type, PNG_INTERLACE_NONE,
				 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, info_ptr);


	/* write bytes */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		log->Error("Can't save PNG file '%s': Error during writing bytes\n", fileName.c_str());
		return false;
	}

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	png_bytep *row_pointers = mnew png_bytep[(unsigned int)image->GetSize().y];

	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < (int)image->GetSize().y; ++i)
		row_pointers[(unsigned int)image->GetSize().y - 1 - i] = (png_bytep)image->getData() + i * rowbytes;

	png_write_image(png_ptr, row_pointers);

	/* end write */
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		log->Error("Can't save PNG file '%s': Error during end of write\n", fileName.c_str());
		return false;
	}

	png_write_end(png_ptr, NULL);

	SafeReleaseArr(row_pointers);

	return true;
}

CLOSE_O2_NAMESPACE
