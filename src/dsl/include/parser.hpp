#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <variant>
#include <optional>
#include <string>
#include "lexer.hpp"
#include <array>

// camelCase -> aliases
// snake_case -> types

namespace grammar {

    using StrOpt = std::string; // idk, more readable for me :)
    struct split_arg
    {
        StrOpt url{};
        int possibility{};
    };

    struct block_arg
    {
        StrOpt url{};
        StrOpt description{};
    };


    struct split_opt { std::vector<split_arg> args; };
    struct target_opt { StrOpt args; };
    struct block_opt { std::vector<block_arg> args; };


    using CaseOpt = std::variant<split_opt, target_opt, block_opt>;
    struct case_opt
    {
        StrOpt opt_str{};
        CaseOpt args{};
    };


    using RuleOpt = std::vector<case_opt>;
    struct rules { RuleOpt args; };


    struct suite
    {
        std::optional<int> port{};
        const rules rules_{};
    };


    struct program { std::vector<suite> conns{}; };


    using ParseOut = std::variant<error_info, program>;
    ParseOut parse_tokens(const lexer& lex);
}

namespace err_decl {

    struct parse_error
    {
        type token_t{};
        std::string_view descr{};

        void print_error() const;
        
        parse_error(type token_t, std::string_view message) : token_t( token_t ), descr( message ) {}
        parse_error(type token_t, const std::exception& err) : token_t( token_t ), descr( err.what() ) {}
    };


    
    enum class error_type
    {
        INVALID_METHOD,
        INVALID_STRING,
        INVALID_VALUE,
        SYNTAX_ERROR,
        MISSING_TOKEN,
        INVALID_TOKEN
    };

    constexpr std::string_view get_error_type_message(error_type err_t) noexcept
    {
        switch( err_t )
        {
            case error_type::INVALID_METHOD: return "Parser does not recognize this method";
            case error_type::INVALID_STRING: return "Parser does not recognize this string value";
            case error_type::INVALID_VALUE: return "Parser does not recognize this value";
            case error_type::SYNTAX_ERROR: return "Parser detected syntax error";
            case error_type::MISSING_TOKEN: return "Parser detected missing token";
            case error_type::INVALID_TOKEN: return "Parser does not recognize this token or the token does not belong here";
        }
        return "error unknown";
    }

}


class parser
{
    token prev_token{};
    tokens_ tokens{};
    size_t curr_pos{};
    friend grammar::ParseOut parse_tokens(const lexer& lex);

public:
    token peek() const;
    bool check(type expect) const;
    bool match(type expect);
    bool check_end() const;
    
    using ConsumeOut = std::variant<err_decl::parse_error, token>;
    ConsumeOut consume(type expect, const err_decl::error_type err_t);
     
    std::optional<int> parse_port();
    grammar::program parse_program();
    grammar::suite parse_suite();
    grammar::rules parse_rules();
    grammar::case_opt parse_case();
    grammar::CaseOpt parse_op();
    grammar::split_opt parse_split();
    grammar::block_opt parse_block();
    grammar::target_opt parse_target();

    bool check_error(ConsumeOut& out) const;
    
    explicit parser(lexer& lex) : tokens( lex.tokenize_source() ) {}
};



#endif
