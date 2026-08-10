#include "include/lexer.hpp"
#include <print>
#include <string_view>
#include <cassert>


constexpr std::string_view token_to_str(type token_type)
{
    for (const auto &elem : mapped) {
        type elem_type = elem.token_type_name;
        if (elem_type == token_type) return elem.type_name;
    }
    return std::string_view{};
}


constexpr type str_to_token(std::string_view str)
{
    for (const auto &elem : mapped) {
        const std::string_view elem_str = elem.type_name;
        if (elem_str == str) return elem.token_type_name;
    }
    
    return type::TOKEN_UNKNOWN;
}


void lexer::print_tokens() const
{
    for (const token &tok : tokens) {
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



bool lexer::is_whitespace() const
{
    char curr_char = peek();
    return (curr_char == '\t' || curr_char == ' ' || curr_char == '\r');
}


bool lexer::is_newline() const
{
    return peek() == '\n';
}

void lexer::skip_whitespaces()
{

    while (!check_eof()) {
        if ( is_whitespace() ) position ++;

        else if ( is_newline() ) {
            line_pos ++;
            position ++;
        } else break;
    }
}


void lexer::advance()
{
    if ( check_eof() ) return;

    position ++;
}

[[nodiscard]] token lexer::tokenize_token()
{
    std::string buff{};
    while ( !is_whitespace() && !check_eof() && !is_newline() ) {
        char curr = peek();
        buff.push_back(curr);
        advance();
    }
    
    type token_type_ = str_to_token(buff);
    std::string_view t_name = token_to_str(token_type_);

    return token{ token_type_, t_name }; 
}

[[nodiscard]] tokens_ lexer::tokenize_source()
{
    tokens_ tokens{};
    while ( !check_eof() ) {
        skip_whitespaces();

        if ( check_eof() ) break;
        tokens.push_back( tokenize_token() );
    }

    return tokens;
}












