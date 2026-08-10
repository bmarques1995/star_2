#include "Value.hh"
#include "TokenType.hh"
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

star::OutOfRangeException::OutOfRangeException(const std::string& reason)
{
    m_Reason = reason;
}

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
}

void star::Value::ParseFloatNumber(std::string_view lexeme)
{
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

void star::Value::ParseChar(std::string_view lexeme)
{

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
    catch(std::out_of_range e)
    {
        if(lexeme.find("-") != SIZE_MAX)
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
            case FloatType::F64: m_Value = std::stod(radical.data()); break;
            default: break;
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
        int64_t value = std::stoll(radical.data());
        if((value < limit_it->second.first) && (value > limit_it->second.second))
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
            case IntegerType::I64: m_Value = value; break;
            default: break;
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
        uint64_t value = std::stoull(radical.data());
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
            case IntegerType::U64: m_Value = value; break;
            default: break;
        }
    }
}
