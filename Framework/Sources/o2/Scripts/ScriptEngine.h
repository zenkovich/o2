#pragma once

#if IS_SCRIPTING_SUPPORTED

#include "o2/Scripts/ScriptValue.h"
#include "o2/Utils/Singleton.h"
#include "o2/Utils/Types/String.h"

#if defined(SCRIPTING_BACKEND_JERRYSCRIPT)
#include "o2/Scripts/JerryScript/ScriptEngineBase.h"
#endif

// Scripts engine access macros
#define  o2Scripts o2::ScriptEngine::Instance()

namespace o2
{
	class ScriptValue;
	struct ScriptConstructorTypeProcessor;

	// -------------------
	// Script parse result
	// -------------------
	class ScriptParseResult: public ScriptParseResultBase
	{
	public:
		// Returns is result ok
		bool IsOk() const;

		// Returns error description
		String GetError() const;

		// Bool cast operator, returns is result ok
		operator bool() const;
	};

	// ----------------------------------------------------------------
	// Scripting engine. Depends on scripting backend. 
	// Can parse, run and evaluate scripts. Can return global namespace
	// ----------------------------------------------------------------
	class ScriptEngine: public Singleton<ScriptEngine>, public ScriptEngineBase
	{
	public:
		// Default constructor, initializes scripting engine
		ScriptEngine();

		// Default destructor, cleans up
		~ScriptEngine();

		// Parses script from string and returns parse result
		ScriptParseResult Parse(const String& script);

		// Runs parsed script, and returns retult
		ScriptValue Run(const ScriptParseResult& parseResult);

		// Evaluates script
		ScriptValue Eval(const String& script);

		// Returns global namespace
		ScriptValue GetGlobal() const;

		// Collects garbage
		void CollectGarbage() const;

	private:
		// Registers all types from reflection
		void RegisterTypes();

		// Runs built in script with math and etc, required to work framework
		void RunBuildtinScripts();

	private:
		typedef void(*RegisterConstructorFunc)(void*, ScriptConstructorTypeProcessor&);
		static Vector<RegisterConstructorFunc>& GetRegisterConstructorFuncs();

		friend class Reflection;
	};
}

#endif // IS_SCRIPTING_SUPPORTED
