#include "Parser.hh"
#include "Expr.hh"
#include "TokenType.hh"
#include "Visitor.hh"
#include <memory>
#include <sstream>
#define inner_assert(E)

const std::unordered_map<std::string, star::VariableType> star::Parser::s_TypeKeywords = 
{
    {"null", VariableType::Null},
    {"bool", VariableType::Boolean},
    
    {"char", VariableType::Character},
    {"string", VariableType::String},
	
    {"i8", VariableType::Integer8},
    {"i16", VariableType::Integer16},
    {"i32", VariableType::Integer32},
    {"i64", VariableType::Integer64},

    {"u8", VariableType::Unsigned8},
	{"u16", VariableType::Unsigned16},
	{"u32", VariableType::Unsigned32},
	{"u64", VariableType::Unsigned64},

    
    {"f32", VariableType::Float32},
	{"f64", VariableType::Float64},

	{"void", VariableType::Void}
};

const std::unordered_map<star::TokenType, star::TokenType> star::Parser::s_BinaryOperators =
{
    {TokenType::REC_PLUS, TokenType::PLUS},
    {TokenType::REC_MINUS, TokenType::MINUS},
	{TokenType::REC_STAR, TokenType::STAR},
	{TokenType::REC_MOD, TokenType::MOD},
	{TokenType::REC_SLASH, TokenType::SLASH}
};

star::ParserException::ParserException(const std::string& message)
{
    m_Reason = "[Parser]: " + message;
}

star::Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) 
{

}

std::vector<std::shared_ptr<star::Statement::Stmt>> star::Parser::Parse()
{
    std::vector<std::shared_ptr<star::Statement::Stmt>> statements;
    while (!IsAtEnd())
    {
        statements.push_back(Declaration());
    }
    return statements;
}

std::shared_ptr<star::Expression::Expr> star::Parser::Expression()
{
    return Assignment();
}

std::shared_ptr<star::Expression::Expr> star::Parser::Equality()
{
    std::shared_ptr<Expression::Expr> expr = Comparison();
    while(Match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)){
        Token oper = Previous();
        std::shared_ptr<Expression::Expr> right = Comparison();
        expr = std::make_shared<Expression::Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<star::Expression::Expr> star::Parser::Comparison()
{
    std::shared_ptr<Expression::Expr> expr = Term();
    while(Match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL))
    {
        Token oper = Previous();
        std::shared_ptr<Expression::Expr> right = Term();
        expr = std::make_shared<Expression::Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<star::Expression::Expr> star::Parser::Term()
{
    std::shared_ptr<Expression::Expr> expr = Factor();
    while(Match(TokenType::MINUS, TokenType::PLUS))
    {
        Token oper = Previous();
        std::shared_ptr<Expression::Expr> right = Factor();
        expr = std::make_shared<Expression::Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<star::Expression::Expr> star::Parser::Factor()
{
    std::shared_ptr<Expression::Expr> expr = Unary();
    while(Match(TokenType::SLASH, TokenType::STAR, TokenType::MOD))
    {
        Token oper = Previous();
        std::shared_ptr<Expression::Expr> right = Unary();
        expr = std::make_shared<star::Expression::Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<star::Expression::Expr> star::Parser::Unary()
{
    while(Match(TokenType::BANG, TokenType::MINUS))
    {
        Token oper = Previous();
        std::shared_ptr<Expression::Expr> right = Unary();
        return std::make_shared<star::Expression::Unary>(oper, right);
    }
    //return Primary();
	return Call();
}

std::shared_ptr<star::Expression::Expr> star::Parser::Primary()
{
    if(Match(TokenType::IDENTIFIER))
        return std::make_shared<star::Expression::Variable>(Previous());
    if(Match(TokenType::TEMPLATE_STRING_START))
        return TemplateLiteral();
    if(Match(TokenType::NUMBER, TokenType::STRING, TokenType::FLOAT_NUMBER, 
        TokenType::ST_TRUE, TokenType::ST_FALSE))
    {
        Value v{Previous().GetTokenType(), Previous().GetLexeme()};
        return std::make_shared<star::Expression::Literal>(v);
    }

    if(Match(TokenType::LEFT_PAREN))
    {
        std::shared_ptr<Expression::Expr> expr = Expression();
        Consume(TokenType::RIGHT_PAREN, "Expected ')' after expression.");
        return std::make_shared<Expression::Grouping>(expr);
    }
    
    std::stringstream ss;
    ss << "Expected expression " << Peek().ToString();
    throw ParserException(ss.str());
}

std::shared_ptr<star::Expression::Expr> star::Parser::Ternary()
{
    std::shared_ptr<Expression::Expr> expr = LogicalOr();

    if (Match(TokenType::QUESTION))
    {
        std::shared_ptr<Expression::Expr> thenBranch = Expression();

        Consume(
            TokenType::COLON,
            "Expected ':' after then branch of conditional expression."
        );

        std::shared_ptr<Expression::Expr> elseBranch = Expression();

        expr = std::make_shared<star::Expression::Ternary>(
            expr,
            thenBranch,
            elseBranch
        );
    }

    return expr;
}

std::shared_ptr<star::Expression::Expr> star::Parser::TemplateLiteral()
{
    std::vector<Expression::TemplateShard> shards;

    while (!Check(TokenType::TEMPLATE_STRING_END) && !IsAtEnd())
    {
        if (Match(TokenType::TEMPLATE_SUBSTRING))
        {
            shards.emplace_back(Previous().GetLexeme());
        }
        else if (Match(TokenType::STR_EXPR_START))
        {
			std::shared_ptr<Expression::Expr> expression = Expression();
            std::string fmt = "";

            Consume(
                TokenType::STR_EXPR_END,
                "Expected '}$' after template expression."
            );

			if (Match(TokenType::TEMPLATE_FMT_START))
			{

                fmt = Consume(
                    TokenType::TEMPLATE_FMT,
                    "Expected template format specifier."
                ).GetLexeme();

                Consume(
                    TokenType::TEMPLATE_FMT_END,
                    "Expected '}@' after template format specifier."
                );
			}

            shards.emplace_back(std::make_pair(std::move(expression), fmt));
        }
        else
        {
            std::stringstream ss;

            ss << "Expected template substring or expression, with optional format specifier right after an expression."
               << Peek().ToString();

            throw ParserException(ss.str());
        }
    }

	if (IsAtEnd())
	{
		throw ParserException("Unterminated template string.");
	}

    Consume(
        TokenType::TEMPLATE_STRING_END,
        "Expected '`' after template string."
    );

    return std::make_shared<star::Expression::TemplateLiteral>(shards);
}

std::shared_ptr<star::Expression::Expr> star::Parser::Assignment()
{

    std::shared_ptr<Expression::Expr> expr = Ternary();

    if (Match(TokenType::EQUAL, TokenType::REC_PLUS, TokenType::REC_MINUS,
        TokenType::REC_STAR, TokenType::REC_SLASH, TokenType::REC_MOD)) {

        Token oper = Previous();
        std::shared_ptr<Expression::Expr> value = Assignment();

        if (auto* variable = dynamic_cast<Expression::Variable*>(expr.get()))
        {
            Token name = variable->m_Name;

            if (oper.m_Type != TokenType::EQUAL) {
                auto it = s_BinaryOperators.find(oper.m_Type);
                TokenType binaryType = it == s_BinaryOperators.end() ? TokenType::EQUAL : it->second;
                value = std::make_shared<Expression::Binary>(
					std::make_shared<Expression::Variable>(name),
					Token(binaryType, oper.m_Lexeme, oper.m_Line, oper.m_Column, oper.m_Filepath),
					value
				);
            }

            return std::make_shared<Expression::Assignment>(
				variable->m_Name,
				value
			);
        }

        throw ParserException("Invalid assignment target.");
    }

    return expr;
}

std::shared_ptr<star::Expression::Expr> star::Parser::LogicalOr()
{
    std::shared_ptr<Expression::Expr> expr = LogicalAnd();
    while (Match(TokenType::OR))
    {
        Token oper = Previous();
        std::shared_ptr<star::Expression::Expr> right = LogicalAnd();
        expr = std::make_shared<star::Expression::Logical>(expr, std::move(oper), right);
    }
    return expr;
}

std::shared_ptr<star::Expression::Expr> star::Parser::LogicalAnd()
{
    std::shared_ptr<Expression::Expr> expr = Equality();
    while (Match(TokenType::AND)) {
        Token oper = Previous();
        std::shared_ptr<Expression::Expr> right = Equality();
        expr = std::make_shared<star::Expression::Logical>(expr, std::move(oper), right);
    }
    return expr;
}

std::shared_ptr<star::Expression::Expr> star::Parser::Call()
{
    std::shared_ptr<star::Expression::Expr> expr = Primary();
	while (Match(TokenType::LEFT_PAREN))
	{
		expr = FinishCall(expr);
	}
	return expr;
}

std::shared_ptr<star::Expression::Expr> star::Parser::FinishCall(std::shared_ptr<Expression::Expr> callee)
{
	std::vector<std::shared_ptr<Expression::Expr>> arguments;
	if (!Check(TokenType::RIGHT_PAREN))
	{
		do
		{
			if (arguments.size() >= 255)
			{
				throw ParserException("Can't exceed 255 arguments.");
			}
			arguments.push_back(Assignment());
		} while (Match(TokenType::COMMA));
	}
	Token paren = Consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments.");

	return std::make_shared<Expression::Call>(callee, std::move(paren), arguments);
}

std::shared_ptr<star::Statement::Stmt> star::Parser::Statement()
{
    if(Match(TokenType::PRINT)) return PrintStatement();
    if(Match(TokenType::IF)) return IfStatement();
    if(Match(TokenType::RETURN)) return ReturnStatement();
	if(Match(TokenType::WHILE)) return WhileStatement();
    if(Match(TokenType::FOR)) return ForStatement();
    //if(Match(TokenType::SWITCH)) return SwitchStatement();
    //if(Match(TokenType::BREAK)) return BreakStatement();
    //if(Match(TokenType::CONTINUE)) return ContinueStatement();
    //if(Match(TokenType::FUNCTION)) return Function("function");
    //if(Match(TokenType::TRY)) return TryStatement();
    //if(Match(TokenType::THROW)) return ThrowStatement();
    //if(Match(TokenType::FOREACH)) return ForEachStatement();
	if(Match(TokenType::LEFT_BRACE)) return std::make_shared<Statement::Block>(Block());
    else return ExpressionStatement();
}

std::shared_ptr<star::Statement::Stmt> star::Parser::PrintStatement()
{
    std::shared_ptr<Expression::Expr> value = Expression();
    Consume(TokenType::SEMICOLON, "Expected ; after value.");
    return std::make_shared<Statement::Print>(value);
}

std::shared_ptr<star::Statement::Stmt> star::Parser::ExpressionStatement()
{
    std::shared_ptr<Expression::Expr> expr = Expression();
    Consume(TokenType::SEMICOLON, "Expected ; after value.");
    return std::make_shared<Statement::Expression>(expr);
}

std::shared_ptr<star::Statement::Stmt> star::Parser::Declaration()
{
    try
    {
        if(Match(TokenType::VAR)) return VarDeclaration();
        else if (Match(TokenType::AUTO)) return AutoDeclaration();
		//refactor this to use enums instead of strings
        else if(Match(TokenType::FUN)) return Function("function");
		else return Statement();
    }
    catch (const ParserException& e)
    {
        Synchronize();
        throw e;
    }
    catch (const std::exception& e)
    {
        Synchronize();
        return nullptr;
    }
}

std::shared_ptr<star::Statement::Stmt> star::Parser::VarDeclaration()
{
    Token name = Consume(TokenType::IDENTIFIER, "Expected variable name.");

	std::shared_ptr<Expression::Expr> init = nullptr;
    VariableType type = MatchHashtag();
    if (Match(TokenType::EQUAL))
    {
        init = Expression();
    }
    Consume(TokenType::SEMICOLON, "Expected ; after variable declaration.");
	return std::make_shared<Statement::Variable>(name, init, type);
}

std::shared_ptr<star::Statement::Stmt> star::Parser::AutoDeclaration()
{
    Token name = Consume(TokenType::IDENTIFIER, "Expected variable name.");
    std::shared_ptr<Expression::Expr> init = nullptr;
    if (Match(TokenType::EQUAL))
    {
        init = Expression();
    }
    Consume(TokenType::SEMICOLON, "Expected ; after variable declaration.");
    return std::make_shared<Statement::Variable>(name, init, true);
}

std::vector<std::shared_ptr<star::Statement::Stmt>> star::Parser::Block()
{
    std::vector<std::shared_ptr<Statement::Stmt>> statements;
	while (!Check(TokenType::RIGHT_BRACE) && !IsAtEnd())
	{
		statements.push_back(Declaration());
	}
	Consume(TokenType::RIGHT_BRACE, "Expected '}' after block.");
    return statements;
}

std::shared_ptr<star::Statement::Stmt> star::Parser::IfStatement()
{
    Consume(TokenType::LEFT_PAREN, "Expected '(' after 'if'.");
	std::shared_ptr<Expression::Expr> condition = Expression();
	Consume(TokenType::RIGHT_PAREN, "Expected ')' after condition.");
	std::shared_ptr<Statement::Stmt> thenBranch = Statement();
	std::shared_ptr<Statement::Stmt> elseBranch = nullptr;
	if (Match(TokenType::ELSE))
	{
		elseBranch = Statement();
	}
	return std::make_shared<Statement::If>(condition, thenBranch, elseBranch);
}

std::shared_ptr<star::Statement::Stmt> star::Parser::WhileStatement()
{
    Consume(TokenType::LEFT_PAREN, "Expected '(' after 'while'.");
    std::shared_ptr<Expression::Expr> condition = Expression();
    Consume(TokenType::RIGHT_PAREN, "Expected ')' after 'while' condition.");
    std::shared_ptr<Statement::Stmt> body = Statement();
    return std::make_shared<Statement::While>(condition, body);
}

std::shared_ptr<star::Statement::Stmt> star::Parser::ForStatement()
{
    Consume(TokenType::LEFT_PAREN, "Expected '(' after 'for'.");

    std::shared_ptr<Statement::Stmt> init;
    if (Match(TokenType::SEMICOLON)) {
        init = nullptr;
    }
    else if (Match(TokenType::VAR)) {
        init = VarDeclaration();
    }
    else {
        init = ExpressionStatement();
    }

    std::shared_ptr<Expression::Expr> condition = nullptr;
    if (!Check(TokenType::SEMICOLON)) {
        condition = Expression();
    }
    Consume(TokenType::SEMICOLON, "Expected ';' after for condition.");

    std::shared_ptr<Expression::Expr> increment = nullptr;
    if (!Check(TokenType::RIGHT_PAREN)) {
        increment = Expression();
    }
    Consume(TokenType::RIGHT_PAREN, "Expected ')' after loop condition.");

    std::shared_ptr<Statement::Stmt> body = Statement();
    if (increment != nullptr) {
        body = std::make_shared<Statement::Block>(
            std::vector<std::shared_ptr<Statement::Stmt>> {
            body, std::make_shared<Statement::Expression>(increment)
        }
        );
    }

    if (condition == nullptr)
    {
        Value v{TokenType::ST_TRUE, ""};
        condition = std::make_shared<Expression::Literal>(v);
    }
    body = std::make_shared<Statement::While>(condition, body);

    if (init != nullptr) {
        body = std::make_shared<Statement::Block>(
            std::vector<std::shared_ptr<Statement::Stmt>>{
            init, body
        }
        );
    }

    return body;
}

std::shared_ptr<star::Statement::Stmt> star::Parser::ReturnStatement()
{
	Token keyword = Previous();
	std::shared_ptr<Expression::Expr> value = nullptr;
	if (!Check(TokenType::SEMICOLON))
	{
		value = Expression();
	}
	Consume(TokenType::SEMICOLON, "Expected ; after return value.");
	return std::make_shared<Statement::Return>(keyword, value);
}

std::shared_ptr<star::Statement::Function> star::Parser::Function(const std::string& kind)
{
#ifdef DISPLAY_IMPROVEMENTS
#error "improve this code to use kind parameter as an enum class"
#endif
	Token functionName = Consume(TokenType::IDENTIFIER, "Expected " + kind + " name.");
	
    std::vector<std::shared_ptr<Statement::FunctionArgument>> parameters;

	Consume(TokenType::LEFT_PAREN, "Expected '(' after function name.");
    if (!Check(TokenType::RIGHT_PAREN))
    {
        do
        {
            if (parameters.size() >= 255)
            {
                throw ParserException("Can't exceed 255 arguments.");
            }
			Token paramName = Consume(TokenType::IDENTIFIER, "Expected parameter name.");
            VariableType paramType = MatchHashtag();
			parameters.push_back(std::make_shared<Statement::FunctionArgument>(paramName, paramType));

        } while (Match(TokenType::COMMA));
    }
    Consume(TokenType::RIGHT_PAREN, "Expected ')' after function name.");
	VariableType returnType = MatchHashtag();
    Consume(TokenType::LEFT_BRACE, "Expected '{' after function declaration.");
    std::vector<std::shared_ptr<Statement::Stmt>> body = Block();
    return std::make_shared<Statement::Function>(functionName, parameters, body, returnType);
}

star::VariableType star::Parser::MatchHashtag()
{
    VariableType paramType = VariableType::Dynamic;
    if (Match(TokenType::HASHTAG))
    {
        Token typeToken = Consume(TokenType::IDENTIFIER, "Expected parameter type.");
        auto it = s_TypeKeywords.find(typeToken.GetLexeme());
        if (it == s_TypeKeywords.end())
            throw ParserException("Invalid parameter type: " + typeToken.GetLexeme());
        paramType = it->second;
    }
    return paramType;
}

template<class...T>
bool star::Parser::Match(T...types)
{
    inner_assert((... && std::is_same_v<T, TokenType>)); 
    if((... || Check(types)))
    {
        Advance();
        return true;
    }
    return false;
}

star::Token star::Parser::Consume(const TokenType& token, const std::string& message)
{
    if(Check(token)) return Advance();
    std::stringstream ss;
    ss << message << Peek().ToString();
    throw ParserException(ss.str());
}

bool star::Parser::Check(const TokenType& type)
{
    if(IsAtEnd()) return false;
    return Peek().m_Type == type;
}

bool star::Parser::IsAtEnd()
{
    return Peek().m_Type == TokenType::ST_EOF;
}

star::Token star::Parser::Advance()
{
    if(!IsAtEnd()) current++;
    return Previous();
}

star::Token star::Parser::Peek()
{
    return tokens.at(current);
}

star::Token star::Parser::Previous()
{
    return tokens.at(current - 1);
}

void star::Parser::Synchronize()
{
    Advance();
    while(!IsAtEnd())
    {
        if(Previous().m_Type == TokenType::SEMICOLON) return;
        switch (Peek().m_Type)
        {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
            default:
                return;
        }
    }
    Advance();
}
