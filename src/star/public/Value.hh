#pragma once

#include "BaseException.hh"
#include "StarMacro.hh"
#include "TokenType.hh"
#include <variant>
#include <cstdint>
#include <string>

namespace star
{
    class STAR_API OutOfRangeException : public ScriptException
    {
    public:
        OutOfRangeException(const std::string& reason);
        ~OutOfRangeException() = default;
    };

    class STAR_API InvalidOperation : public ScriptException
    {
    public:
        InvalidOperation(const std::string& reason);
        ~InvalidOperation() = default;
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

        friend star::Value& operator-(star::Value& value);

        Value(TokenType type, std::string_view lexeme);
        Value(const Storage& value);
        ~Value() = default;

        const std::string ToString() const;
        Storage& GetLValue();
        const Storage& GetRValue() const;
        bool IsInitialized() const;
        bool IsNumber() const;

    private:
        void ParseFloatNumber(std::string_view lexeme);
        void ParseNumber(std::string_view lexeme);
        void ParseString(std::string_view lexeme);
        //void ParseChar(std::string_view lexeme);

        void InferInteger(std::string_view lexeme);

        void AssignTypedFloat(std::string_view lexeme, size_t typeOffset);
        void AssignTypedInt(std::string_view lexeme, size_t typeOffset);
        void AssignTypedUint(std::string_view lexeme, size_t typeOffset);
        Storage m_Value;
    };

    STAR_API std::ostream& operator<<(std::ostream& out, const star::Value& value);
    STAR_API star::Value& operator-(star::Value& value);
    STAR_API star::Value& operator+(star::Value& value1, const star::Value& value2);
    STAR_API star::Value& operator-(star::Value& value1, const star::Value& value2);
    STAR_API star::Value& operator*(star::Value& value1, const star::Value& value2);
    STAR_API star::Value& operator/(star::Value& value1, const star::Value& value2);
    STAR_API star::Value& operator%(star::Value& value1, const star::Value& value2);
    STAR_API bool operator==(const star::Value& value1, const star::Value& value2);
    STAR_API bool operator<(const star::Value& value1, const star::Value& value2);
    STAR_API bool operator<=(const star::Value& value1, const star::Value& value2);
    STAR_API bool operator>(const star::Value& value1, const star::Value& value2);
    STAR_API bool operator>=(const star::Value& value1, const star::Value& value2);
}
