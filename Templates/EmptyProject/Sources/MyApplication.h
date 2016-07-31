#pragma once

#include "Application/Application.h"
#include "Utils/IObject.h"
#include "Utils/Log/LogStream.h"

using namespace o2;

namespace MyApp
{
	class Tmp: public IObject
	{
	public:
		IOBJECT(Tmp);
	};

	// ---------------------
	// Code tool application
	// ---------------------
	class MyApplication: public Application
	{

	public:
		// Default constructor. Initializes all editor components
		MyApplication();

		// Destructor
		~MyApplication();

	protected:

	protected:
		// Calling when application is starting
		void OnStarted();

		// Calling when application is closing
		void OnClosing();

		// Calling on updating
		void OnUpdate(float dt);

		// Calling on drawing
		void OnDraw();
	};
}