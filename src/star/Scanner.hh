#pragma once

#include <cstddef>
#include "RegexProcessor.hh"
//#include <iostream>
#include <vector>
#include <unordered_map>
#include "Token.hh"
#include "StarMacro.hh"
#include "BaseException.hh"
#include <exception>

/*
* TODO add line, column and file information to the token.
*/

namespace star
{

    class STAR_API ScannerException : public ScriptException, public std::exception
    {
    public:
        ScannerException(const std::string& reason, size_t line = 0);
        ~ScannerException() = default;

        const char* what() const noexcept override;
    };

    class STAR_API Scanner
    {
    private:
        size_t m_Start;
        size_t m_Current;
        size_t m_Line;
        
        std::string m_Source;
        std::vector<Token> m_Tokens;
        static const std::unordered_map<std::string, TokenType> s_Keywords;
    
        bool IsAlpha(char c);
        bool IsDigit(char c);

        bool Match(char expected);
        void ScanToken();
        char Advance();

        void AddToken(TokenType type);
        void AddToken(TokenType type, std::any literal);

        char Peek();

        void ProcessSlash(char c);
        void ProcessDefault(char c);

        void ProcessComment();
        void ProcessMultilineComment();

        bool ProcessInteger();
        bool ProcessHex();
        bool ProcessFloat();

        void String();
        void Number();
        void Identifier();

        void InitCurrentProcessingString(std::string_view* currentText);
    public:
        Scanner(const std::string& source);
        ~Scanner() = default;

        bool IsAtEnd();
        const std::vector<Token>& ScanTokens();

        RegexProcessor m_LCProcessor;
        RegexProcessor m_MLCProcessor;
        RegexProcessor m_IdentProcessor;
        RegexProcessor m_IntProcessor;
        RegexProcessor m_HexProcessor;
        RegexProcessor m_FloatProcessor;
        RegexProcessor m_IsFloat;
        RegexProcessor m_NumberInitIdentProcessor;
    };
}