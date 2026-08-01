#pragma once

#include <cstddef>
#include "RegexProcessor.hh"
//#include <iostream>
#include <vector>
#include <unordered_map>
#include "Token.hh"
#include "StarMacro.hh"
#include "BaseException.hh"

namespace star
{

    class STAR_API ScannerException : public ScriptException
    {
    public:
        ScannerException(const std::string& reason, size_t line = 0);
        ~ScannerException() = default;
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
        bool IsAlphaNumeric(char c);

        bool Match(char expected);
        void ScanToken();
        char Advance();

        void AddToken(TokenType type);
        void AddToken(TokenType type, std::any literal);

        char Peek();
        char PeekNext();

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