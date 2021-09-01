#pragma once

#ifdef PLATFORM_MAC

namespace o2
{
	// -------------------------------
	// Android application base fields
	// -------------------------------
	class ApplicationBase
	{
    protected:
        Vec2I mResolution;


	public:

		// Returns android data path
		String GetDataPath() const;
    };
}

#endif // PLATFORM_MAC
