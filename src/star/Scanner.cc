#include "Scanner.hh"
#include "Token.hh"
#include "TokenType.hh"
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <algorithm>
#include <unordered_map>

using match_range = std::pair<size_t, size_t>;

star::ScannerException::ScannerException(const std::string& reason, size_t line) : ScriptException()
{
    std::string lineInfo = line > 0 ? " at line: " + std::to_string(line) : "";
    m_Reason = "ScannerException: " + reason + lineInfo;
}

const std::unordered_map<std::string, star::TokenType> star::Scanner::s_Keywords =
{
    {"and",    TokenType::AND},
    {"class",  TokenType::CLASS},
    {"else",   TokenType::ELSE},
    {"false",  TokenType::ST_FALSE},
    {"for",    TokenType::FOR},
    {"fun",    TokenType::FUN},
    {"if",     TokenType::IF},
    {"nil",    TokenType::NIL},
    {"or",     TokenType::OR},
    {"print",  TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super",  TokenType::SUPER},
    {"this",   TokenType::THIS},
    {"true",   TokenType::ST_TRUE},
    {"var",    TokenType::VAR},
    {"while",  TokenType::WHILE}
};

namespace star_definitions
{
    const std::string s_OperatorPost = R"((?=[\+\-\*\/\%=;\!\<\>\&\|\(\) \t\r\n]|\z))";
    //(?=[\+\-\*\/\%=;\!\<\>\&\| \t\r\n]|\z)
    const std::string s_CommentRegex = R"(\/\/[^\r\n]*)";
    const std::string s_MultilineCommentRegex = R"((?s)\/\*.*?(?:\*\/|$))";
    const std::string s_IdentifierRegex = R"(^[a-zA-Z_][a-zA-Z0-9_]*)";
    const std::string s_HexRegex = R"(\A(?<number>(0[xX][0-9a-fA-F]+)?)(?<type>i(?:8|16|32|64)|u(?:8|16|32|64))?)" + s_OperatorPost;
    const std::string s_IntRegex = R"(\A(?<number>([0-9](?:[0-9]|(?:'[0-9]{3}))*))(?<type>i(?:8|16|32|64)|u(?:8|16|32|64))?)" + s_OperatorPost;
    const std::string s_FloatRegex = R"(\A(?<number>(?:[0-9]+(?:\.[0-9]*)?|\.[0-9]+)(?:[eE][+-]?[0-9]+)?)(?<type>f(?:32|64))?)" + s_OperatorPost;
    const std::string s_IsFloat = R"([eEfF.])";
    const std::string s_NumberInitIdentRegex = R"(^[0-9][a-zA-Z0-9_]*)";
}

bool star::Scanner::IsAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || 
    (c >= 'A' && c <= 'Z') || 
    (c == '_');
}

bool star::Scanner::IsDigit(char c)
{
    return (c >= '0') && (c <= '9');
}

bool star::Scanner::Match(char expected)
{
    if(IsAtEnd() || m_Source.at(m_Current) != expected)
        return false;
    m_Current++;
    return true;
}

void star::Scanner::ScanToken()
{
    char c = Advance();
    switch(c){
        case '(': AddToken(TokenType::LEFT_PAREN); break;
        case ')': AddToken(TokenType::RIGHT_PAREN); break;
        case '{': AddToken(TokenType::LEFT_BRACE); break;
        case '}': AddToken(TokenType::RIGHT_BRACE); break;
        case ',': AddToken(TokenType::COMMA); break;
        case '#': AddToken(TokenType::NUM_SIGN); break;
        case '.':
            if(IsDigit(Peek()))
            {
                Number();
            }
            else
            {
                AddToken(TokenType::DOT);
            }
            break;
        case '-': AddToken(Match('=') ? TokenType::REC_MINUS : TokenType::MINUS); break;
        case '+': AddToken(Match('=') ? TokenType::REC_PLUS : TokenType::PLUS); break;
        case ';': AddToken(TokenType::SEMICOLON); break;
        case '*': AddToken(Match('=') ? TokenType::REC_STAR : TokenType::STAR); break;
        case '%': AddToken(Match('=') ? TokenType::REC_MOD : TokenType::MOD); break;
        case '[': AddToken(TokenType::LEFT_BRACKET); break;
        case ']': AddToken(TokenType::RIGHT_BRACKET); break;
        case '!':
            AddToken(Match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=':
            AddToken(Match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<':
            AddToken(Match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>':
            AddToken(Match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            ProcessSlash(c);
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            m_Line++;
            break;
        case '"':
            String();
            break;
        default:
            ProcessDefault(c);
            break;
    } 
}

char star::Scanner::Advance()
{
    return m_Source[m_Current++];
}

void star::Scanner::AddToken(TokenType type, const std::string& lexeme)
{
    std::string text{m_Source.substr(m_Start, m_Current - m_Start)};
    if(lexeme != "")
        text = lexeme;
    size_t column = m_Start - m_LineBreaks[m_Line - 1];
    m_Tokens.emplace_back(type, text, m_Line, column, m_Filepath);
}

char star::Scanner::Peek()
{
    if(IsAtEnd())
        return '\0';
    return m_Source.at(m_Current);
}

void star::Scanner::ProcessSlash(char c)
{
    if(Match('/')){
        ProcessComment();
    }
    else if(Match('*'))
    {
        ProcessMultilineComment();
    }
    else{
        AddToken(Match('=') ? TokenType::REC_SLASH : TokenType::SLASH);
    }
}

void star::Scanner::ProcessDefault(char c)
{
    if(IsDigit(c) || c == '.')
    {
        Number();
    }
    else if(IsAlpha(c))
    {
        Identifier();
    }
    else
    {
        throw ScannerException(std::string("Unexpected character: ") + c, m_Line);
    }
}

void star::Scanner::ProcessComment()
{
    std::string_view currentProcessing;
    InitCurrentProcessingString(&currentProcessing);

    match_range match = m_LCProcessor.GetFirstMatch(currentProcessing); 
    m_Current = m_Start + match.second;
}

void star::Scanner::ProcessMultilineComment()
{
    std::string_view currentProcessing;
    InitCurrentProcessingString(&currentProcessing);

    match_range match = m_MLCProcessor.GetFirstMatch(currentProcessing);
    std::string text{m_Source.substr(m_Start, match.second)};
    size_t newLineIncidences = std::count(text.begin(), text.end(), '\n');
    m_Current = m_Start + match.second;
    m_Line += newLineIncidences;
}

char star::Scanner::ProcessEscapedChar(char evaluated)
{
    static const std::unordered_map<char, char> escapeMap =
    {
        {'\\', '\\'},
        {'n', '\n'},
        {'r', '\r'},
        {'t', '\t'},
        {'%', '%'},
        {'$', '$'},
        {'{', '{'},
        {'}', '}'},
    };
    auto it = escapeMap.find(evaluated);
    if(it != escapeMap.end())
        return it->second;
    else
        throw ScannerException("The only valid escape sequences are: '\\\\', '\\n', '\\r', '\\t', '\\%', '\\$', '\\{' and '\\}'");
}

void star::Scanner::String()
{
    char c = Peek();
    std::vector<char> processedString;
    while(c != '\"' && !IsAtEnd())
    {
        if(c == '\\')
        {
            Advance();
            c = ProcessEscapedChar(Peek());
        }
        if(c == '\n')
            m_Line++;
        processedString.push_back(c);
        Advance();
        c = Peek();
    }

    if(IsAtEnd())
    {
        throw ScannerException("Unterminated string, ", m_Line);
    }

    Advance();

    std::string value{processedString.begin(), processedString.end()};
    AddToken(TokenType::STRING, value);
}

bool star::Scanner::ProcessInteger()
{
    std::string_view currentProcessing;
    InitCurrentProcessingString(&currentProcessing);

    match_range match = m_IntProcessor.GetFirstMatch(currentProcessing);
    if(match.first == match.second)
        return false;
    m_Current = m_Start + match.second;
    return true;
}

bool star::Scanner::ProcessHex()
{
    std::string_view currentProcessing;
    InitCurrentProcessingString(&currentProcessing);

    match_range match = m_HexProcessor.GetFirstMatch(currentProcessing);
    if(match.first == match.second)
        return false;
    m_Current = m_Start + match.second;
    return true;
}

bool star::Scanner::ProcessFloat()
{
    std::string_view currentProcessing;
    InitCurrentProcessingString(&currentProcessing);

    match_range match = m_FloatProcessor.GetFirstMatch(currentProcessing);
    if(match.first == match.second)
        return false;
    m_Current = m_Start + match.second;
    return true;
}

void star::Scanner::Number()
{
    char c = m_Source.at(m_Start);
    TokenType type = TokenType::NUMBER;
    if((c == '.') && IsDigit(Peek()))
    {
        if(!ProcessFloat())
        {
            throw ScannerException("Invalid float literal", m_Line);
        }
        type = TokenType::FLOAT_NUMBER;
    }
    if(c == '0' && (Peek() == 'x' || Peek() == 'X'))
    {
        if(!ProcessHex())
        {
            throw ScannerException("Invalid hex literal", m_Line);
        }
    }
    else if(IsDigit(c))
    {
        bool isInteger = ProcessInteger();
        bool isFloat = ProcessFloat();

        if(!isInteger && !isFloat)
        {
            throw ScannerException("Invalid number literal", m_Line);
        }
        if(isFloat)
            type = TokenType::FLOAT_NUMBER;
    }

    AddToken(type);
}

void star::Scanner::Identifier()
{
    std::string_view currentProcessing;
    InitCurrentProcessingString(&currentProcessing);

    match_range match = m_IdentProcessor.GetFirstMatch(currentProcessing);
    std::string text{m_Source.substr(m_Start, match.second)};
    auto it = s_Keywords.find(text);
    TokenType type = it == s_Keywords.end() ? TokenType::IDENTIFIER : it->second;
    m_Current = m_Start + match.second;
    AddToken(type);
}

void star::Scanner::InitCurrentProcessingString(std::string_view* currentText)
{
    auto strIt = m_Source.begin();
    strIt += m_Start;
    *currentText = std::string_view(strIt, m_Source.end());
}

star::Scanner::Scanner(const std::string& source, const std::string& filepath) : 
    m_Source(source), m_Filepath(filepath),
    m_Start(0), m_Current(0), m_Line(1),
    m_HexProcessor{star_definitions::s_HexRegex},
    m_FloatProcessor{star_definitions::s_FloatRegex},
    m_IntProcessor{star_definitions::s_IntRegex},
    m_IsFloat{star_definitions::s_IsFloat},
    m_IdentProcessor{star_definitions::s_IdentifierRegex},
    m_LCProcessor{star_definitions::s_CommentRegex},
    m_MLCProcessor{star_definitions::s_MultilineCommentRegex},
    m_NumberInitIdentProcessor{R"()"}
{
    m_LineBreaks.push_back(0);
    for(size_t i = 0; i < m_Source.length(); i++)
        if(m_Source.at(i) == '\n')
            m_LineBreaks.push_back(i);
}

bool star::Scanner::IsAtEnd()
{
    return m_Current >= static_cast<uint32_t>(m_Source.length( ));
}

const std::vector<star::Token>& star::Scanner::ScanTokens()
{
    while(!IsAtEnd())
    {
        m_Start = m_Current;
        ScanToken();
    }

    m_Tokens.emplace_back(TokenType::ST_EOF, "", m_Line + 1, 0, m_Filepath);
    return m_Tokens;
}
