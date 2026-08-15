#pragma once

#include "StarMacro.hh"
#include <string>

namespace star
{
    class STAR_API Output
    {
    public:
        virtual ~Output() = default;
        virtual void Write(const std::string& text) const = 0;
    };
}