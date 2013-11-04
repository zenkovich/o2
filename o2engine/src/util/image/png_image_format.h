#ifndef PNG_IMAGE_FORMAT_H
#define PNG_IMAGE_FORMAT_H

#include "public.h"

OPEN_O2_NAMESPACE

class cImage;
class cLogStream;

bool loadPngImage(const std::string& fileName, cImage* image, bool errors = true, cLogStream* plog = NULL);
bool savePngImage(const std::string& fileName, const cImage* image, cLogStream* plog = NULL);

CLOSE_O2_NAMESPACE

#endif //PNG_IMAGE_FORMAT_H