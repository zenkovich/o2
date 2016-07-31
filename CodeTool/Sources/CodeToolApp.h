#pragma once

#include "Application/Application.h"
#include "Utils/IObject.h"
#include "Utils/Log/LogStream.h"

using namespace o2;

namespace CodeTool
{
	class Tmp: public IObject
	{
	public:
		IOBJECT(Tmp);
	};

	// ---------------------
	// Code tool application
	// ---------------------
	class CodeToolApplication: public Application
	{

	public:
		// Default constructor. Initializes all editor components
		CodeToolApplication();

		// Destructor
		~CodeToolApplication();

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