#pragma once

#include "BaseException.hh"
#include "StarMacro.hh"
#include "TokenType.hh"
#include <variant>
#include <cstdint>
#include <string>
#include <memory>
#include <unordered_map>

namespace star
{
	struct Function;
	enum class VariableType
	{
		Null,
		Boolean,

		Character,
		String,

		Function,

		Integer8,
		Integer16,
		Integer32,
		Integer64,

		Unsigned8,
		Unsigned16,
		Unsigned32,
		Unsigned64,

		Float32,
		Float64,

		Void,
		Dynamic
	};

	class STAR_API OutOfRangeException : public ScriptException
	{
		friend class Environment;
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
		friend class Interpreter;
		friend class Environment;
		friend class Function;
	public:
		using Storage = std::variant<
			std::monostate,
			bool,

			char8_t,
			std::string,

			std::shared_ptr<Function>,

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
		Value(const Storage& value, bool lockType);
		~Value() = default;

		const std::string ToString(const std::string& format = "") const;
		const std::string StringifyString() const;
		Storage& GetLValue();
		const Storage& GetRValue() const;
		bool IsInitialized() const;
		bool IsNumber() const;

		VariableType GetType() const;
		VariableType GetAssignedType() const;

	private:
		void ParseFloatNumber(std::string_view lexeme);
		void ParseNumber(std::string_view lexeme);
		void ParseString(std::string_view lexeme);
		//void ParseChar(std::string_view lexeme);


		void LockType();

		void InferInteger(std::string_view lexeme);

		void AssignTypedFloat(std::string_view lexeme, size_t typeOffset);
		void AssignTypedInt(std::string_view lexeme, size_t typeOffset);
		void AssignTypedUint(std::string_view lexeme, size_t typeOffset);
		
		Storage m_Value;
		VariableType m_Type;
		static const std::unordered_map<size_t, VariableType> castedType;
	};

	std::ostream& operator<<(std::ostream& out, const star::Value& value);
	star::Value& operator-(star::Value& value);
	star::Value& operator+(star::Value& value1, const star::Value& value2);
	star::Value& operator-(star::Value& value1, const star::Value& value2);
	star::Value& operator*(star::Value& value1, const star::Value& value2);
	star::Value& operator/(star::Value& value1, const star::Value& value2);
	star::Value& operator%(star::Value& value1, const star::Value& value2);
	bool operator==(const star::Value& value1, const star::Value& value2);
	bool operator<(const star::Value& value1, const star::Value& value2);
	bool operator<=(const star::Value& value1, const star::Value& value2);
	bool operator>(const star::Value& value1, const star::Value& value2);
	bool operator>=(const star::Value& value1, const star::Value& value2);
}
