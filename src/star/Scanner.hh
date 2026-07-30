#pragma once

#include <cstdint>
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
        uint32_t m_Start;
        uint32_t m_Current;
        uint32_t m_Line;
        
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

        void String();
        void Number();
        void Identifier();

    public:
        Scanner(const std::string& source);
        bool IsAtEnd();
        std::vector<Token> ScanTokens();
    };
}