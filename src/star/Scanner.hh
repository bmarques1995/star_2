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
        std::vector<size_t> m_LineBreaks;

        std::string m_Source;
        std::vector<Token> m_Tokens;
        std::string m_Filepath;
        static const std::unordered_map<std::string, TokenType> s_Keywords;
    
        bool IsAlpha(char c);
        bool IsDigit(char c);

        bool Match(char expected);
        void ScanToken();
        char Advance();
        void MultiAdvance(size_t offset);
        void AdvanceAndCommit(size_t offset);
        void CommitAdvance();

        void AddToken(TokenType type, const std::string& lexeme = "");

        char Peek(size_t offset = 0);

        void ProcessSlash(char c);
        void ProcessDefault(char c);

        void ProcessComment();
        void ProcessMultilineComment();

        bool ProcessInteger();
        bool ProcessHex();
        bool ProcessFloat();

        char ProcessEscapedChar(char evaluated);

        void TemplateString();
        void String();
        void Number();
        void Identifier();

        void InitCurrentProcessingString(std::string_view* currentText);
    public:
        Scanner(const std::string& source, const std::string& filepath = "::repl");
        bool IsAtEnd(size_t offset = 0);
        const std::vector<Token>& ScanTokens();

        static const std::unordered_map<char, char> s_EscapeMap;
        static const std::unordered_map<char, char> s_ReverseEscapeMap;

    private:
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