#pragma once
#include <variant>

namespace star
{
    namespace value_helper
    {
        template<typename T>
        void Negate(T& value)
        {
            if constexpr (std::is_arithmetic_v<T> &&
                        !std::is_same_v<T, bool>)
            {
                value = -value;
            }
        }

        template<typename T1, typename T2>
        void Add(T1& lhs, const T2& rhs)
        {
            if constexpr (std::is_arithmetic_v<T1> &&
                        std::is_arithmetic_v<T2>)
            {
                lhs += rhs;
            }
            else if constexpr (
                std::is_same_v<T1, std::string> &&
                std::is_same_v<T2, std::string>)
            {
                lhs += rhs;
            }
            else
            {
                throw InvalidOperation("You can't add arithmetic with strings and is only accepted number or strings");
            }
        }
 
        template<typename T1, typename T2>
        void Subtract(T1& lhs, const T2& rhs)
        {
            if constexpr (std::is_arithmetic_v<T1> &&
                        std::is_arithmetic_v<T2>)
            {
                lhs -= rhs;
            }
            else
            {
                throw InvalidOperation("You can only subtract arithmetic");
            }
        }

        template<typename T1, typename T2>
        void Multiply(T1& lhs, const T2& rhs)
        {
            if constexpr (std::is_arithmetic_v<T1> &&
                        std::is_arithmetic_v<T2>)
            {
                lhs *= rhs;
            }
            else
            {
                throw InvalidOperation("You can only multiply arithmetic");
            }
        }

        template<typename T1, typename T2>
        void Divide(T1& lhs, const T2& rhs)
        {
            if constexpr (std::is_arithmetic_v<T1> &&
                        std::is_arithmetic_v<T2>)
            {
                lhs /= rhs;
            }
            else
            {
                throw InvalidOperation("You can only divide arithmetic");
            }
        }

        template<typename T1, typename T2>
        void Module(T1& lhs, const T2& rhs)
        {
            if constexpr (std::is_integral_v<T1> &&
                        std::is_integral_v<T2>)
            {
                lhs %= rhs;
            }
            else
            {
                throw InvalidOperation("You can only get module of integers");
            }
        }
    }
}