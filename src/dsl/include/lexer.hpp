#ifndef LEXER_H
#define LEXER_H
#include <string_view>
#include <vector>
#include <string>
#include <exception>


enum class type
{
    TOKEN_SUITE,
    TOKEN_PORT,
    TOKEN_RULES,
    TOKEN_CASE,
    TOKEN_SPLIT,
    TOKEN_TARGET,
    TOKEN_VALUE,
    TOKEN_PERCENT,
    TOKEN_PIPE,
    TOKEN_EOF,
    TOKEN_EQUALS,
    TOKEN_COLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_SEMICOLON
};


struct token
{
    type token_type{};
    std::string_view lexeme{};
    token(type token_type, std::string_view lexeme) :
        token_type(token_type), lexeme(lexeme) {} 
};


struct error_info
{
    type token_type{};
    std::string description{};
    size_t pos{ 1 };

    error_info() = default; 
    error_info(type token_type, const std::exception &err, size_t pos) : 
        token_type(token_type), description(err.what()), pos(pos) {}
};


class lexer
{
    std::string_view source{};
    size_t position{};
    size_t line_pos{ 1 };
    
public:
    std::vector<token> tokens{};    
    
    template<type Token>
    [[nodiscard]] token token_init(std::string_view lexeme = "") const
    {
        return token{ Token, lexeme };
    }

    template<type Token>
    [[nodiscard]] error_info error_init(const std::exception &err, size_t pos) const 
    {
        return error_info{ Token, err, pos };
    }

    void print_tokens() const;
    void advance();
    char read_curr_char();
    char peek() const;
    void skip_whitespaces();
    [[nodiscard]] bool check_eof() const;
    std::vector<token> tokenize();

    explicit lexer(std::string_view source) : source(source) {}
};

constexpr std::string_view token_to_str(type type_);

#endif
