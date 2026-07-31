#pragma once

#include <cstddef>
#include "RegexProcessor.hh"
//#include <iostream>
#include <vector>
#include <unordered_map>
#include "Token.hh"
#include "StarMacro.hh"
//#include "Debug.hh"

namespace star
{
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
        std::string RegexAdvance(size_t offset);

        void AddToken(TokenType type);
        void AddToken(TokenType type, std::any literal);

        char Peek();
        char PeekNext();

        void ProcessSlash(char c);
        void ProcessDefault(char c);

        void ProcessComment();
        void ProcessMultilineComment();

        void ProcessIdentifier();
        void ProcessInteger();
        void ProcessHex();
        void ProcessFloat();

        void String();
        void Number();
        void Identifier();

    public:
        Scanner(const std::string& source);
        bool IsAtEnd();
        std::vector<Token> ScanTokens();
        RegexProcessor m_LCProcessor;
        RegexProcessor m_MLCProcessor;
        RegexProcessor m_IdentProcessor;
        RegexProcessor m_IntProcessor;
        RegexProcessor m_HexProcessor;
        RegexProcessor m_FloatProcessor;
        RegexProcessor m_IsFloat;
    };
}