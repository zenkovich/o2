#pragma once

namespace o2
{
    // ----------------------------------------------------------------------
    // Linked reference. Used to advanced referecing to Actor or Component, 
    // that keeps link inside hierarchy after instantiation and serialization
    // ----------------------------------------------------------------------
    template<typename _type, typename _enable = void>
    class LinkRef {};
}
