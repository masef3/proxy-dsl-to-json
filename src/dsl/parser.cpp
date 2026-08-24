#include "include/parser.hpp"
#include "include/lexer.hpp"
#include <vector>
#include <type_traits>
#include <algorithm>
#include <iostream>
#include <variant>
#include <print>
#include <utility>


bool parser::check_error(ConsumeOut& out) const
{
    if (std::holds_alternative<err_decl::parse_error>(out)) {
        auto& err = std::get<err_decl::parse_error>(out);
        std::println("{} : {}", err.token_t, err.descr);
        return true;
    }
    return false;
}


bool parser::check_end() const
{
    return ((curr_pos >= tokens.size()) || (tokens[ curr_pos ].token_type == type::TOKEN_EOF));
}


token parser::peek() const
{
    if (check_end()) return tokens.back();
    return tokens[ curr_pos ];
}


bool parser::check(type expect) const
{
    if (check_end()) return false;
    return peek().token_type == expect;
}


bool parser::match(type expect)
{
    if (!check( expect )) return false; 
    prev_token = peek();
    return tokens[ curr_pos ++ ].token_type == expect;
}


parser::ConsumeOut parser::consume(type expect, const err_decl::error_type err_t)
{
    if (match( expect )) return peek();
    
    return err_decl::parse_error{ expect, get_error_type_message(err_t) };
}


std::optional<int> parser::parse_port()
{
    if ( peek().token_type != type::TOKEN_PORT ) return std::nullopt;

    auto out = parser::consume(type::TOKEN_PORT, err_decl::error_type::INVALID_TOKEN);
    if (check_error(out)) return std::nullopt;

    out = parser::consume(type::TOKEN_EQUALS, err_decl::error_type::SYNTAX_ERROR);
    if (check_error(out)) return std::nullopt;

    const token& tok = peek();
    if (std::all_of(tok.lexeme.begin(), tok.lexeme.end(), ::isdigit)) {
        out = parser::consume(type::TOKEN_VALUE, err_decl::error_type::INVALID_VALUE);
        if (check_error(out)) return std::nullopt;
        return std::stoi(tok.lexeme);
    }
}


grammar::target_opt parser::parse_target()
{
    auto out = parser::consume(type::TOKEN_TARGET, err_decl::error_type::INVALID_TOKEN);
    if (check_error(out)) return {};

    out = parser::consume(type::TOKEN_STRING, err_decl::error_type::INVALID_STRING);
    if (check_error(out)) return {};

    return grammar::target_opt{ std::move(parser::prev_token.lexeme) };
}


grammar::block_opt parser::parse_block()
{
    auto out = parser::consume(type::TOKEN_BLOCK, err_decl::error_type::INVALID_TOKEN);
    if (check_error(out)) return {};
    std::vector<std::string_view> args{};

    while (peek().token_type == type::TOKEN_STRING) {
        out = parser::consume(type::TOKEN_STRING, err_decl::error_type::INVALID_STRING);
        if (check_error(out)) return {};

        args.emplace_back(std::string_view{ parser::prev_token.lexeme });
    }

    return grammar::block_opt{ args };
}


grammar::split_arg parser::parse_split_arg(bool trigger)
{
    auto out = parser::consume(type::TOKEN_STRING, err_decl::error_type::INVALID_STRING);
    if (check_error(out)) {
        trigger = true;
        return {};
    }

    std::string url = std::move(parser::prev_token.lexeme);

    out = parser::consume(type::TOKEN_EQUALS, err_decl::error_type::SYNTAX_ERROR);
    if (check_error(out)) {
        trigger = true;
        return {};
    }

    out = parser::consume(type::TOKEN_VALUE, err_decl::error_type::INVALID_VALUE);
    if (check_error(out)) {
        trigger = true;
        return {};
    }

    // sorry for redundancy :(

    return grammar::split_arg{ url, std::stoi(parser::prev_token.lexeme) };
}


grammar::split_opt parser::parse_split()
{
    auto out = parser::consume(type::TOKEN_SPLIT, err_decl::error_type::INVALID_TOKEN);
    if (check_error(out)) return {};
    std::vector<grammar::split_arg> args{};
    bool trigger = false;
    
    for (auto sout = parse_split_arg(trigger); peek().token_type != type::TOKEN_STRING || trigger; sout = parse_split_arg(trigger)) {
        args.push_back(sout); 
    }
    
    if (trigger) return {};
    return grammar::split_opt{ args };
}





