#include "Value.hh"
#include "ValueOperations.hh"


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
