#pragma once

#include "o2/Utils/Types/String.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace o2
{
    class UID
    {
    public:
        char data[16];

    public:
        UID();
        UID(const UID& other);
        explicit UID(int value);
        explicit UID(const WString& stringData);

        UID& operator=(const UID& other);

        bool operator==(const UID& other) const;
        bool operator!=(const UID& other) const;

        bool operator<(const UID& other) const;

        void Randomize();

        WString ToString() const;
        void FromString(const WString& stringData);

        explicit operator WString() const;
        UID& operator=(const WString& data);

        explicit operator String() const;
        UID& operator=(const String& data);

    public:
        static UID empty;
    };
}
