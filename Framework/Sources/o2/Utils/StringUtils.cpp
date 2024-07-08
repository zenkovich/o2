#include "o2/stdafx.h"
#include "StringUtils.h"

namespace o2
{
    String GetSmartName(const String& name)
    {
        String begn;

        if (name[0] == 'm' && name[1] >= 'A' && name[1] <= 'Z')
            begn = name.SubStr(1);
        else if (name[0] == 'm' && name[1] == '_')
            begn = name.SubStr(2);
        else if (name[0] == '_')
            begn = name.SubStr(1);
        else
            begn = name;

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