#include "o2/stdafx.h"
#include "CommandLineOptions.h"

namespace o2
{
    void CommandLineOptions::Parse(int argc, char** argv, IObject& output)
    {
        Map<String, String> data = Parse(argc, argv);
        
        auto& type = dynamic_cast<const ObjectType&>(output.GetType());
        void* originOutputPtr = type.DynamicCastFromIObject(&output);

        for (auto& kv : data)
        {
            if (auto field = type.GetField(kv.first))
            {
                if (field->GetType() == &TypeOf(int))
                    field->SetValue(originOutputPtr, (int)kv.second);
                else if (field->GetType() == &TypeOf(float))
                    field->SetValue(originOutputPtr, (float)kv.second);
                else if (field->GetType() == &TypeOf(String))
                    field->SetValue(originOutputPtr, kv.second);
                else if (field->GetType() == &TypeOf(bool))
                    field->SetValue(originOutputPtr, (bool)kv.second);
            }
        }
    }

    Map<String, String> CommandLineOptions::Parse(int argc, char** argv)
    {
        Map<String, String> result;

        for (int i = 1; i < argc; i += 2)
        {
            String key = argv[i];
            String value = argv[i + 1];
            result[key] = value;
        }

        return result;
    }

}
