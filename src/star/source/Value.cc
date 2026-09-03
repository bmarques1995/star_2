#include "Value.hh"
#include "TokenType.hh"
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <variant>
#include "Scanner.hh"

namespace star
{
    namespace helpers
    {
        std::string StringifyString(const std::string& value)
        {
            std::string result;
            result.reserve(value.size() + 2);

            result += '"';

            for (char c : value)
            {
                auto it = Scanner::s_ReverseEscapeMap.find(c);
                if(it != Scanner::s_ReverseEscapeMap.end())
                {
                    result += '\\'; 
                    result += it->second;
                }
                else
                    result += c;
            }

            result += '"';
            return result;
        }
    }
}

star::OutOfRangeException::OutOfRangeException(const std::string& reason)
{
    m_Reason = reason;
}

star::InvalidOperation::InvalidOperation(const std::string& reason)
{
    m_Reason = reason;
}

const std::unordered_map<size_t, star::VariableType> star::Value::castedType = 
{
	{0, VariableType::Null},
	{1, VariableType::Boolean},
	{2, VariableType::Character},
	{3, VariableType::String},
	{4, VariableType::Integer8},
	{5, VariableType::Integer16},
	{6, VariableType::Integer32},
	{7, VariableType::Integer64},
	{8, VariableType::Unsigned8},
	{9, VariableType::Unsigned16},
	{10, VariableType::Unsigned32},
	{11, VariableType::Unsigned64},
	{12, VariableType::Float32},
	{13, VariableType::Float64}
};

star::Value::Value(TokenType type, std::string_view lexeme)
{
    switch (type)
    {
        case TokenType::NUMBER: ParseNumber(lexeme); break;
        case TokenType::FLOAT_NUMBER: ParseFloatNumber(lexeme); break;
        case TokenType::STRING: ParseString(lexeme); break;
        case TokenType::ST_FALSE: m_Value = false; break;
        case TokenType::ST_TRUE: m_Value = true; break;
        default: break;
    }

	m_Type = VariableType::Dynamic;
}

star::Value::Value(const Storage& value):
    m_Value{ std::move(value) }, m_Type{VariableType::Dynamic}
{

}

star::Value::Value(const Storage& value, bool lockType) :
    m_Value{ std::move(value) }
{
    if(lockType)
	    LockType();
}

bool star::Value::IsInitialized() const
{
    return !std::holds_alternative<std::monostate>(m_Value);
}

bool star::Value::IsNumber() const
{
    return std::visit(
        [](const auto& value)
        {
            using T = std::decay_t<decltype(value)>;
            return std::is_arithmetic_v<T>;
        },
        m_Value
    );
}

star::VariableType star::Value::GetType() const
{
    return m_Type;
}

star::VariableType star::Value::GetAssignedType() const
{
    auto it = castedType.find(m_Value.index());
    return it != castedType.end() ? it->second : VariableType::Dynamic;
}

void star::Value::ParseFloatNumber(std::string_view lexeme)
{
    //npos
    auto typeAssignment = lexeme.find("f");
    if(typeAssignment == SIZE_MAX)
        m_Value = std::stod(lexeme.data());
    else
        AssignTypedFloat(lexeme, typeAssignment);
}

void star::Value::ParseNumber(std::string_view lexeme)
{
    size_t intAssignment = lexeme.find("i");
    size_t uintAssignment = lexeme.find("u");
    if((intAssignment == SIZE_MAX) && (uintAssignment == SIZE_MAX))
    {
        //test - and value
        InferInteger(lexeme);
        return;
    }
    uintAssignment != SIZE_MAX ? AssignTypedUint(lexeme, uintAssignment) : AssignTypedInt(lexeme, intAssignment);

}

void star::Value::ParseString(std::string_view lexeme)
{
    std::string finalValue = lexeme.data();
    m_Value = finalValue;
}

/*
void star::Value::ParseChar(std::string_view lexeme)
{

}
*/

void star::Value::LockType()
{
	auto it = castedType.find(m_Value.index());
	m_Type = it != castedType.end() ? it->second : VariableType::Dynamic;
}

void star::Value::InferInteger(std::string_view lexeme)
{
    int64_t intTry;
    uint64_t uintTry;
    bool signedWorked = true;
    try
    {
        intTry = std::stoll(lexeme.data());
        if((intTry >= static_cast<int64_t>(INT32_MIN)) && (intTry <= static_cast<int64_t>(INT32_MAX)))
            m_Value = static_cast<int32_t>(intTry);
        else
            m_Value = intTry;
        return;
    }
    catch(const std::out_of_range& e)
    {
        if(lexeme.find("-") != std::string_view::npos)
            throw OutOfRangeException("Minimum value is -9223372036854775808");
    }

    try
    {
        uintTry = std::stoull(lexeme.data());
        m_Value = uintTry;
    }
    
    catch(std::out_of_range e)
    {
        throw OutOfRangeException("Maximum value is 18446744073709551615");
    }
}

void star::Value::AssignTypedFloat(std::string_view lexeme, size_t typeOffset)
{
    auto divisor = lexeme.begin();
    divisor += (typeOffset);
    std::string_view radical{lexeme.begin(), divisor};
    std::string_view typeAssignment{divisor, lexeme.end()};
    static const std::unordered_map<std::string_view, FloatType> castedType = 
    {
        {"f32", FloatType::F32},
        {"f64", FloatType::F64}
    };
    auto it = castedType.find(typeAssignment);
    if(it != castedType.end())
    {
        switch (it->second) 
        {
            case FloatType::F32: m_Value = std::stof(radical.data()); break;
            case FloatType::F64: default: m_Value = std::stod(radical.data()); break;
        }
    }
}

void star::Value::AssignTypedInt(std::string_view lexeme, size_t typeOffset)
{
    auto divisor = lexeme.begin();
    divisor += (typeOffset);
    std::string_view radical{lexeme.begin(), divisor};
    std::string_view typeAssignment{divisor, lexeme.end()};
    static const std::unordered_map<std::string, IntegerType> castedType = 
    {
        {"i8", IntegerType::I8},
        {"i16", IntegerType::I16},
        {"i32", IntegerType::I32},
        {"i64", IntegerType::I64}
    };
    static const std::unordered_map<IntegerType, std::pair<int64_t,int64_t>> intLimits = 
    {
        {IntegerType::I8, {INT8_MIN, INT8_MAX}},
        {IntegerType::I16, {INT16_MIN, INT16_MAX}},
        {IntegerType::I32, {INT32_MIN, INT32_MAX}},
        {IntegerType::I64, {INT64_MIN, INT64_MAX}}
    };
    auto type_it = castedType.find(typeAssignment.data());
    auto limit_it = type_it != castedType.end() ? intLimits.find(type_it->second) : intLimits.end();

    if(limit_it != intLimits.end())
    {
        int64_t value;
        
        try
        {
            value = std::stoll(radical.data());
        }
        catch(const std::out_of_range& e)
        {
            std::stringstream ss;
            ss << "Supported range for language is {" << INT64_MIN << "," << INT64_MAX << "} for signed integers"; 
            throw OutOfRangeException(ss.str());
        }

        if((value < limit_it->second.first) || (value > limit_it->second.second))
        {
            std::stringstream ss;
            ss << "Supported range for " << typeAssignment << " is {" << limit_it->second.first << "," << limit_it->second.second << "}"; 
            throw OutOfRangeException(ss.str());
        }
        switch (limit_it->first)
        {
            case IntegerType::I8: m_Value = static_cast<int8_t>(value); break;
            case IntegerType::I16: m_Value = static_cast<int16_t>(value); break;
            case IntegerType::I32: m_Value = static_cast<int32_t>(value); break;
            case IntegerType::I64: default: m_Value = value; break;
        }
    }
}

void star::Value::AssignTypedUint(std::string_view lexeme, size_t typeOffset)
{
    auto divisor = lexeme.begin();
    divisor += (typeOffset);
    std::string_view radical{lexeme.begin(), divisor};
    std::string_view typeAssignment{divisor, lexeme.end()};
    static const std::unordered_map<std::string, IntegerType> castedType = 
    {
        {"u8", IntegerType::U8},
        {"u16", IntegerType::U16},
        {"u32", IntegerType::U32},
        {"u64", IntegerType::U64}
    };
    static const std::unordered_map<IntegerType, uint64_t> uintLimits = 
    {
        {IntegerType::U8, UINT8_MAX},
        {IntegerType::U16, UINT16_MAX},
        {IntegerType::U32, UINT32_MAX},
        {IntegerType::U64, UINT64_MAX}
    };
    auto type_it = castedType.find(typeAssignment.data());
    auto limit_it = type_it != castedType.end() ? uintLimits.find(type_it->second) : uintLimits.end();

    if(limit_it != uintLimits.end())
    {
        uint64_t value;
        
        try
        {
            value = std::stoull(radical.data());
        }
        catch(const std::out_of_range& e)
        {
            std::stringstream ss;
            ss << "Supported range for language is {0," << UINT64_MAX << "} for unsigned integers"; 
            throw OutOfRangeException(ss.str());
        }

        if(value > limit_it->second)
        {
            std::stringstream ss;
            ss << "Supported range for " << typeAssignment << " is {0," << limit_it->second << "}"; 
            throw OutOfRangeException(ss.str());
        }
        switch (limit_it->first)
        {
            case IntegerType::U8: m_Value = static_cast<uint8_t>(value); break;
            case IntegerType::U16: m_Value = static_cast<uint16_t>(value); break;
            case IntegerType::U32: m_Value = static_cast<uint32_t>(value); break;
            case IntegerType::U64: default: m_Value = value; break;
        }
    }
}

const std::string star::Value::ToString() const
{
    return std::visit(
        [this](const auto& value) -> std::string
        {
            using T = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<T, std::monostate>)
            {
                return "null";
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                return value ? "true" : "false";
            }
            else if constexpr (std::is_same_v<T, char8_t>)
            {
                return std::string(1, static_cast<char>(value));
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                return value;
            }
            else if constexpr (std::is_same_v<T, int8_t>)
            {
                return std::to_string(static_cast<int>(value));
            }
            else if constexpr (std::is_same_v<T, uint8_t>)
            {
                return std::to_string(static_cast<unsigned int>(value));
            }
            else
            {
                return std::to_string(value);
            }
        },
        m_Value
    );
}

const std::string star::Value::StringifyString() const
{
    return std::visit(
        [](const auto& value) -> std::string
        {
            using T = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<T, std::monostate>)
            {
                return "null";
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                return helpers::StringifyString(value);
            }
            else
            {
                return std::to_string(value);
            }
        },
        m_Value
    );
}

star::Value::Storage& star::Value::GetLValue()
{
    return m_Value;
}

const star::Value::Storage& star::Value::GetRValue() const
{
    return m_Value;
}

std::ostream& star::operator<<(std::ostream& out, const star::Value& value)
{
    out << value.ToString();
    return out;
}
