#pragma once

#include "o2/Utils/Types/CommonTypes.h"
#include "o2/Utils/Serialization/Serializable.h"
#include "o2/Utils/Types/String.h"

namespace o2
{
	// ---------------------------------------------------
	// Command line options, parsing options by reflection
	// ---------------------------------------------------
	class CommandLineOptions
	{
	public:
		// Parses data to map
		static Map<String, String> Parse(int argc, char** argv);

		// Parses data to output object
		static void Parse(int argc, char** argv, IObject& output);
	};
}
