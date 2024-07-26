#pragma once

#include <boost/stacktrace.hpp>

namespace o2
{
    // Returns stack trace as text
    //String GetStackTrace();

    using StackTrace = boost::stacktrace::stacktrace;
}
