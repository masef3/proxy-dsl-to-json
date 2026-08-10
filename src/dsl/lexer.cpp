#include "include/lexer.hpp"
#include <print>
#include <string_view>


constexpr std::string_view token_to_str(type type_)
{
    switch(type_) {
        case type::TOKEN_SUITE: return "TOKEN_SUITE";
        case type::TOKEN_PORT: return "TOKEN_PORT";
        case type::TOKEN_RULES: return "TOKEN_RULES";
        case type::TOKEN_CASE: return "TOKEN_CASE";
        case type::TOKEN_SPLIT: return "TOKEN_SPLIT";
        case type::TOKEN_TARGET: return "TOKEN_TARGET";
        case type::TOKEN_VALUE: return "TOKEN_VALUE";
        case type::TOKEN_PERCENT: return "TOKEN_PERCENT";
        case type::TOKEN_PIPE: return "TOKEN_PIPE";
        case type::TOKEN_EOF: return "TOKEN_EOF";
        case type::TOKEN_EQUALS: return "TOKEN_EQUALS";
        case type::TOKEN_COLON: return "TOKEN_COLON";
        case type::TOKEN_LPAREN: return "TOKEN_LPAREN";
        case type::TOKEN_RPAREN: return "TOKEN_RPAREN";
        case type::TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
        default:                    return "TOKEN_UNKNOWN";
    }
}


using namespace lexer;

void lexer::print_tokens() const
{
    for (token tok : tokens) {
        std::println("{} : {}", tok.lexeme, token_to_str(tok.token_type));
    }
}


[[nodiscard]] bool lexer::check_eof() const
{
    return (position >= source.size());    
}


char lexer::peek() const
{
    if (check_eof()) return '\0';
    return source[ position ];
}


char lexer::read_curr_char()
{
    if (check_eof()) return '\0';
    return source[ position ++ ];
}


void lexer::skip_whitespaces()
{
    char curr_char = peek();
    while (!check_eof()) {
        if (curr_char == '\t' || curr_char == ' ' || curr_char == '\r') {
            curr_char = read_curr_char();
        } else if (curr_char == '\n') {
            line_pos ++;
            curr_char = read_curr_char();
        } else {
            break;
        }
    }
}


















