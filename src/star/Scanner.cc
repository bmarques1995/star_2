#include "Scanner.hh"
#include "Token.hh"
#include "TokenType.hh"
#include "Debug.hh"
#include <cstddef>
#include <cstdint>
#include <string_view>

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

bool star::Scanner::IsAlphaNumeric(char c)
{
    return IsAlpha(c) || IsDigit(c);
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
        case '.': AddToken(TokenType::DOT); break;
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
            break;
        case '"':
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

std::string star::Scanner::RegexAdvance(size_t offset)
{
    std::string result = m_Source.substr(m_Start, offset);
    m_Current = m_Start + offset;
    return result;
}

void star::Scanner::AddToken(TokenType type)
{
    AddToken(type, nullptr);
}

void star::Scanner::AddToken(TokenType type, std::any literal)
{
    std::string text{m_Source.substr(m_Start, m_Current - m_Start)};
    m_Tokens.emplace_back(type, text, literal, m_Line);
}

char star::Scanner::Peek()
{
    if(IsAtEnd())
        return '\0';
    return m_Source.at(m_Current);
}
char star::Scanner::PeekNext()
{
    if((m_Current + 1) > static_cast<uint32_t>(m_Source.length()))
        return '\0';
    return m_Source.at(m_Current + 1);
}

void star::Scanner::ProcessSlash(char c)
{
    if(Match('/')){
        while(Peek() != '\n' && !IsAtEnd()){
            Advance();
        }
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
        Debug::Error(m_Line, "Unexpected character.");
    }
}

void star::Scanner::String()
{
    while(Peek() != '\"' && !IsAtEnd())
    {
        if(Peek() == '\n')
            m_Line++;
        Advance();
    }

    if(IsAtEnd())
    {
        Debug::Error(m_Line, "Unterminated string.");
        return;
    }

    Advance();

    std::string value{m_Source.substr(m_Start + 1, m_Current - m_Start + 1)};
    AddToken(TokenType::STRING, value);
}

void star::Scanner::Number()
{
    while(IsDigit(Peek()))
        Advance();
    if(Peek() == '.' && IsDigit(PeekNext()))
    {
        Advance();
        while(IsDigit(Peek()))
            Advance();
    }
}

void star::Scanner::Identifier()
{
    auto strIt = m_Source.begin();
    strIt += m_Start;
    std::string_view currentProcessing(strIt, m_Source.end());

    std::pair<size_t, size_t> match = m_IdentProcessor.GetFirstMatch(currentProcessing);
    std::string text{m_Source.substr(m_Start, match.second)};
    auto it = s_Keywords.find(text);
    TokenType type = it == s_Keywords.end() ? TokenType::IDENTIFIER : it->second;
    m_Current = m_Start + match.second;
    AddToken(type);
}

star::Scanner::Scanner(const std::string& source) : m_Source(source),
    m_Start(0), m_Current(0), m_Line(1),
    m_HexProcessor{"(0[xX][0-9a-fA-F]+)([ui](?:8|16|32|64))?"},
    m_IntProcessor{"([0-9](?:[0-9]|(?:'[0-9]{3}))*)([ui](?:8|16|32|64))"}, m_IsFloat{"[eEfF.]"}, m_IdentProcessor{"^[a-zA-Z_][a-zA-Z0-9_]*"},
    m_LCProcessor{"\\/\\/[^\r\n]*"}, m_MLCProcessor{"\\/\\*.*?\\*\\/"}, 
    m_FloatProcessor{"([0-9]*\\.[0-9]+(?:[eE][+-]?[0-9]+)?|[0-9]+\\.(?:[0-9]+)?(?:[eE][+-]?[0-9]+)?|[0-9]+(?:[eE][+-]?[0-9]+)?)([fF](?:16|32|64))?"}
{
}

bool star::Scanner::IsAtEnd()
{
    return m_Current >= static_cast<uint32_t>(m_Source.length( ));
}

std::vector<star::Token> star::Scanner::ScanTokens()
{
    while(!IsAtEnd())
    {
        m_Start = m_Current;
        ScanToken();
    }
    m_Tokens.emplace_back(TokenType::ST_EOF, "", nullptr, m_Line);
    return m_Tokens;
}
