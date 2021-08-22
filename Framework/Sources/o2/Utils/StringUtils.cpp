#include "o2/stdafx.h"
#include "StringUtils.h"

namespace o2
{
	String GetSmartName(const String& fieldName)
	{
		String begn;

		if (fieldName[0] == 'm' && fieldName[1] >= 'A' && fieldName[1] <= 'Z')
			begn = fieldName.SubStr(1);
		else if (fieldName[0] == 'm' && fieldName[1] == '_')
			begn = fieldName.SubStr(2);
		else if (fieldName[0] == '_')
			begn = fieldName.SubStr(1);
		else
			begn = fieldName;

		if (begn.StartsWith("o2::"))
			begn.Erase(0, 4);

		if (begn.StartsWith("Editor::"))
			begn.Erase(0, 8);

		if (begn.StartsWith("UI"))
			begn = begn;

		String res;
		int len = begn.Length();
		bool newWord = true;
		bool lastUpper = false;
		for (int i = 0; i < len; i++)
		{
			if (begn[i] == '_')
			{
				res += ' ';
				newWord = true;
				lastUpper = false;
			}
			else if (newWord && begn[i] >= 'a' && begn[i] <= 'z')
			{
				res += begn[i] + ('A' - 'a');
				lastUpper = true;
			}
			else if (!newWord && begn[i] >= 'A' && begn[i] <= 'Z')
			{
				if (!lastUpper)
					res += ' ';

				res += begn[i];
				lastUpper = begn[i] >= 'A' && begn[i] <= 'Z';
			}
			else if (i < len - 1 && begn[i] == ':' && begn[i + 1] == ':')
			{
				res += ": ";
				lastUpper = false;
				i++;
			}
			else
			{
				res += begn[i];
				lastUpper = begn[i] >= 'A' && begn[i] <= 'Z';
			}

			newWord = begn[i] >= '0' && begn[i] <= '9';
		}

		return res;
	}
}