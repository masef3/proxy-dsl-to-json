#include "include/lexer.hpp"
#include <print>
#include <string_view>
#include <cassert>
#include <sstream>
#include <cctype>
#include <algorithm>


std::string_view token_to_str(type token_type)
{
    for (const auto &elem : mapped) {
        type elem_type = elem.token_type_name;
        if (elem_type == token_type) return elem.type_name;
    }
    return std::string_view{};
}


type str_to_token(std::string_view str)
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
    return (curr_char == '\t' || curr_char == ' ' || curr_char == '\r' || curr_char == ',');
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

[[nodiscard]] std::string lexer::get_next_elem()
{
    std::string buff{};
    skip_whitespaces();

    while (!is_whitespace() && !is_newline() && !check_eof() ) {
        buff.push_back(peek());
        advance();
    }

    return buff;
}
[[nodiscard]] tokens_ lexer::tokenize_source()
{
    tokens_ toks{};

    auto get_token = [](std::string_view raw, std::string_view tk_name)
    {
        if (raw == "=") return type::TOKEN_EQUALS;
        if (raw == "%") return type::TOKEN_PERCENT;
        if (raw == "|") return type::TOKEN_PIPE;
        if (raw == "{") return type::TOKEN_LBRACE;
        if (raw == "}") return type::TOKEN_RBRACE;
        if (raw == "(") return type::TOKEN_LPAREN;
        if (raw == ")") return type::TOKEN_RPAREN;
        if (raw == ";") return type::TOKEN_SEMICOLON;
        if (raw == ":") return type::TOKEN_COLON;

        if (raw == "") return type::TOKEN_EOF;
        if (!raw.empty() && std::all_of(raw.begin(), raw.end(), ::isdigit)) return type::TOKEN_VALUE;
        if (raw.size() >= 2 && raw.front() == '"' && raw.back() == '"') return type::TOKEN_STRING;

        for (const type_mapper& tm : mapped) {
            if (tm.type_name == tk_name) return tm.token_type_name;
        }

        return type::TOKEN_UNKNOWN;
    };

    while (!check_eof()) {
        
        std::string tk_str = get_next_elem();
        if (tk_str.size() >= 2 && tk_str.front() == '"' && tk_str.back() == '"') {
            std::string final_string = tk_str.substr(1, tk_str.size() - 2);
            toks.push_back(token_init(type::TOKEN_STRING, final_string));
            continue;
        }

        std::transform(tk_str.begin(), tk_str.end(), tk_str.begin(), [](unsigned char c) { return std::toupper(c); } ); 
        const std::string curr_token_name = "TOKEN_" + tk_str;

        std::transform(tk_str.begin(), tk_str.end(), tk_str.begin(), [](unsigned char c) { return std::tolower(c); } );
    

        toks.push_back(token_init( get_token(tk_str, curr_token_name), tk_str ));
    }
    return toks;
}


