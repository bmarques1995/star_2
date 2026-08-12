#include "Value.hh"
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

star::Value& star::operator-(star::Value& value)
{
    std::visit(
        [](auto& rawValue)
        {
            value_helper::Negate(rawValue);
        },
        value.GetLValue()
    );

    return value;   
}

star::Value& star::operator+(star::Value& value1,
                      const star::Value& value2)
{
    std::visit(
        [](auto& lhs, const auto& rhs) -> void
        {
            using T1 = std::decay_t<decltype(lhs)>;
            using T2 = std::decay_t<decltype(rhs)>;
            value_helper::Add(lhs, rhs);
        },
        value1.GetLValue(),
        value2.GetRValue()
    );
    return value1;
}

star::Value& star::operator-(star::Value& value1,
                      const star::Value& value2)
{
    std::visit(
        [](auto& lhs, const auto& rhs) -> void
        {
            using T1 = std::decay_t<decltype(lhs)>;
            using T2 = std::decay_t<decltype(rhs)>;
            value_helper::Subtract(lhs, rhs);
        },
        value1.GetLValue(),
        value2.GetRValue()
    );
    return value1;
}

STAR_API star::Value& star::operator*(star::Value& value1, const star::Value& value2)
{
    std::visit(
        [](auto& lhs, const auto& rhs) -> void
        {
            using T1 = std::decay_t<decltype(lhs)>;
            using T2 = std::decay_t<decltype(rhs)>;
            value_helper::Multiply(lhs, rhs);
        },
        value1.GetLValue(),
        value2.GetRValue()
    );
    return value1;
}
STAR_API star::Value& star::operator/(star::Value& value1, const star::Value& value2)
{
    std::visit(
        [](auto& lhs, const auto& rhs) -> void
        {
            using T1 = std::decay_t<decltype(lhs)>;
            using T2 = std::decay_t<decltype(rhs)>;
            value_helper::Divide(lhs, rhs);
        },
        value1.GetLValue(),
        value2.GetRValue()
    );
    return value1;
}

STAR_API star::Value& star::operator%(star::Value& value1, const star::Value& value2)
{
    std::visit(
        [](auto& lhs, const auto& rhs) -> void
        {
            using T1 = std::decay_t<decltype(lhs)>;
            using T2 = std::decay_t<decltype(rhs)>;
            value_helper::Module(lhs, rhs);
        },
        value1.GetLValue(),
        value2.GetRValue()
    );
    return value1;
}

bool star::operator==(const star::Value& value1,
                      const star::Value& value2)
{
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> bool
        {
            using T1 = std::decay_t<decltype(lhs)>;
            using T2 = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<T1, T2>)
            {
                return lhs == rhs;
            }
            else
            {
                return false;
            }
        },
        value1.GetRValue(),
        value2.GetRValue()
    );
}

bool star::operator<(const star::Value& value1,
                      const star::Value& value2)
{
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> bool
        {
            using T1 = std::decay_t<decltype(lhs)>;
            using T2 = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<T1, T2>)
            {
                return lhs < rhs;
            }
            else
            {
                return false;
            }
        },
        value1.GetRValue(),
        value2.GetRValue()
    );
}

bool star::operator<=(const star::Value& value1,
                      const star::Value& value2)
{
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> bool
        {
            using T1 = std::decay_t<decltype(lhs)>;
            using T2 = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<T1, T2>)
            {
                return lhs <= rhs;
            }
            else
            {
                return false;
            }
        },
        value1.GetRValue(),
        value2.GetRValue()
    );
}

bool star::operator>(const star::Value& value1,
                      const star::Value& value2)
{
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> bool
        {
            using T1 = std::decay_t<decltype(lhs)>;
            using T2 = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<T1, T2>)
            {
                return lhs > rhs;
            }
            else
            {
                return false;
            }
        },
        value1.GetRValue(),
        value2.GetRValue()
    );
}

bool star::operator>=(const star::Value& value1,
                      const star::Value& value2)
{
    return std::visit(
        [](const auto& lhs, const auto& rhs) -> bool
        {
            using T1 = std::decay_t<decltype(lhs)>;
            using T2 = std::decay_t<decltype(rhs)>;

            if constexpr (std::is_same_v<T1, T2>)
            {
                return lhs >= rhs;
            }
            else
            {
                return false;
            }
        },
        value1.GetRValue(),
        value2.GetRValue()
    );
}
