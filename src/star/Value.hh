#pragma once

#include "BaseException.hh"
#include "StarMacro.hh"
#include "TokenType.hh"
#include <variant>
#include <cstdint>
#include <string>

namespace star
{
    class OutOfRangeException : public ScriptException
    {
    public:
        OutOfRangeException(const std::string& reason);
        ~OutOfRangeException() = default;
    };

    class STAR_API Value
    {
    public:
        using Storage = std::variant<
            std::monostate,
            bool,

            char8_t,
            std::string,

            int8_t,
            int16_t,
            int32_t,
            int64_t,

            uint8_t,
            uint16_t,
            uint32_t,
            uint64_t,

            float,
            double
        >;
    
        Value(TokenType type, std::string_view lexeme); 
    
    private:   

        void ParseFloatNumber(std::string_view lexeme);
        void ParseNumber(std::string_view lexeme);
        void ParseString(std::string_view lexeme);
        void ParseChar(std::string_view lexeme);

        void InferInteger(std::string_view lexeme);

        void AssignTypedFloat(std::string_view lexeme, size_t typeOffset);
        void AssignTypedInt(std::string_view lexeme, size_t typeOffset);
        void AssignTypedUint(std::string_view lexeme, size_t typeOffset);
        Storage m_Value;
    };
}