#pragma once

#include "o2/Utils/Types/String.h"

namespace o2
{
    // Makes smart and readable name from the given name. 
    // Used to get readable names from the type names and other names.
    // - removes 'm' prefix
	// - replaces '::' namespace separator with ' '
	// - reples '_' with ' '
    // - capitalizes the first letter of each word and adds space between words
    String GetSmartName(const String& name);
}
