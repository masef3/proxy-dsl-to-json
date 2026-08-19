#ifndef LEXER_H
#define LEXER_H
#include <string_view>
#include <vector>
#include <string>
#include <exception>
#include <array>


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
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_SEMICOLON,
    TOKEN_ERROR,
    TOKEN_UNKNOWN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_STRING
};


struct token
{
    type token_type{};
    std::string lexeme{};
    token() = default;
    token(type token_type, std::string_view lexeme) :
        token_type(token_type), lexeme(lexeme) {} 
};


using tokens_ = std::vector<token>;


struct error_info
{
    type token_type{};
    std::string description{};
    std::size_t pos{ 1 };

    error_info() = default; 
    error_info(type token_type, const std::exception &err, size_t pos) : 
        token_type(token_type), description(err.what()), pos(pos) {}
};


class lexer
{
    std::string_view source{};
    std::size_t position{};
    std::size_t line_pos{ 1 };
    tokens_ tokens{};
    friend class parser;
    
public:
    
    [[nodiscard]] token token_init(type token_type, std::string_view lexeme = "") const
    {
        return token{ token_type, lexeme };
    }

    template<type Token>
    [[nodiscard]] error_info error_init(const std::exception &err, size_t pos) const 
    {
        return error_info{ Token, err, pos };
    }

    void print_tokens() const;
    void advance();
    bool is_whitespace() const;
    bool is_newline() const;
    char peek() const;
    void skip_whitespaces();
    [[nodiscard]] bool check_eof() const; 
    [[nodiscard]] std::string get_next_elem();
    [[nodiscard]] tokens_ tokenize_source();


    explicit lexer(std::string_view& source) : source(source) {}
};


struct type_mapper
{
    std::string_view type_name{};
    type token_type_name;
};


constexpr std::size_t TOKEN_COUNT = 20;
constexpr std::string_view token_to_str(type token_type);
constexpr type str_to_token(std::string_view str);
using map_ = std::array<type_mapper, TOKEN_COUNT>;
inline constexpr map_ mapped = 
{
    type_mapper{"TOKEN_SUITE",     type::TOKEN_SUITE},
    type_mapper{"TOKEN_PORT",      type::TOKEN_PORT},
    type_mapper{"TOKEN_RULES",     type::TOKEN_RULES},
    type_mapper{"TOKEN_CASE",      type::TOKEN_CASE},
    type_mapper{"TOKEN_SPLIT",     type::TOKEN_SPLIT},
    type_mapper{"TOKEN_TARGET",    type::TOKEN_TARGET},
    type_mapper{"TOKEN_VALUE",     type::TOKEN_VALUE},
    type_mapper{"TOKEN_PERCENT",   type::TOKEN_PERCENT},
    type_mapper{"TOKEN_PIPE",      type::TOKEN_PIPE},
    type_mapper{"TOKEN_EOF",       type::TOKEN_EOF},
    type_mapper{"TOKEN_EQUALS",    type::TOKEN_EQUALS},
    type_mapper{"TOKEN_STRING",    type::TOKEN_STRING},
    type_mapper{"TOKEN_LBRACE",    type::TOKEN_LBRACE},
    type_mapper{"TOKEN_RBRACE",    type::TOKEN_RBRACE},
    type_mapper{"TOKEN_SEMICOLON", type::TOKEN_SEMICOLON},
    type_mapper{"TOKEN_ERROR",     type::TOKEN_ERROR},
    type_mapper{"TOKEN_UNKNOWN",   type::TOKEN_UNKNOWN},
    type_mapper{"TOKEN_LPAREN",    type::TOKEN_LPAREN},
    type_mapper{"TOKEN_RPAREN",    type::TOKEN_RPAREN},

};

#endif
