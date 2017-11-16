#include "Core/EditorApplication.h"
#include "Core/ToolsPanel.h"
#include "O2.h"

#include "Scene/Actor.h"
#include "Render/Camera.h"

using namespace o2;
using namespace Editor;

DECLARE_SINGLETON(WindowsManager);
DECLARE_SINGLETON(EditorConfig);
DECLARE_SINGLETON(ToolsPanel);

#include <map>
#include "Utils/Timer.h"

std::map<std::string, int> testMap =
{
	{ "abcgdef", 0 },
	{ "abcfdef", 1 },
	{ "abdcdef", 2 },
	{ "abcndef", 3 },
	{ "acbcdef", 4 },
	{ "abcdjef", 5 },
	{ "ajbcdef", 6 },
	{ "abcdhef", 7 },
	{ "ahbcdef", 8 },
	{ "abcdhef", 9 },
	{ "hasbcdef", 10 },
	{ "abfhcdef", 11 },
	{ "abscdhef", 12 },
	{ "abfhcdef", 13 },
	{ "abcsdhef", 14 },
	{ "ahfbcdef", 15 },
	{ "adbcdehf", 16 },
	{ "abcfdef", 17 },
	{ "abcdhef", 18 },
	{ "abcadef", 19 },
	{ "abcwdef", 20 }
};

std::vector<std::string> testMap2 =
{
	"abcwdef", 
	"abcadef", 
	"abcdhef", 
	"abcfdef", 
	"adbcdehf", 
	"ahfbcdef", 
	"abcsdhef", 
	"abfhcdef", 
	"abfhcdef", 
	"hasbcdef", 
	"abcdhef",
	"ahbcdef",
	"abcdhef",
	"ajbcdef",
	"abcdjef",
	"acbcdef",
	"abcndef",
	"abdcdef",
	"abcfdef",
	"abcgdef",
};

int Test1(const std::string& name)
{
	return testMap[name];
}

int Test2(const std::string& name)
{
	if (name == "abcgdef") return 0;
	if (name == "abcfdef") return 1;
	if (name == "abdcdef") return 2;
	if (name == "abcndef") return 3;
	if (name == "acbcdef") return 4;
	if (name == "abcdjef") return 5;
	if (name == "ajbcdef") return 6;
	if (name == "abcdhef") return 7;
	if (name == "ahbcdef") return 8;
	if (name == "abcdhef") return 9;
	if (name == "hasbcdef") return 10;
	if (name == "abfhcdef") return 11;
	if (name == "abscdhef") return 12;
	if (name == "abfhcdef") return 13;
	if (name == "abcsdhef") return 14;
	if (name == "ahfbcdef") return 15;
	if (name == "adbcdehf") return 16;
	if (name == "abcfdef") return 17;
	if (name == "abcdhef") return 18;
	if (name == "abcadef") return 19;
	if (name == "abcwdef") return 20;

	return 0;
}

int main()
{
	INITIALIZE_O2;

	EditorApplication* app = mnew EditorApplication();
	app->Launch();
	delete app;

	return 0;
}

