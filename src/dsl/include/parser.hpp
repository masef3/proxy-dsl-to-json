#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <variant>
#include <optional>
#include <string>
#include "lexer.hpp"

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
    struct target_opt { std::vector<StrOpt> args; };
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

struct parse_error
{
    type token_t{};
    std::string descr{};

    parse_error(type token_t, const std::exception& err) : token_t( token_t ), descr( err.what() ) {}
};

class parser
{
    tokens_ tokens{};
    size_t curr_pos{};
    friend grammar::ParseOut parse_tokens(const lexer& lex);

public:
    token peek() const;
    bool check(type expect) const;

    using MatchOut = std::variant<parse_error, bool>;
    MatchOut match(type expect);
    bool check_end() const;
    
    using ConsumeOut = std::variant<error_info, token>;
    ConsumeOut consume(type token_type);
     
    std::optional<int> parse_port();
    grammar::program parse_program();
    grammar::suite parse_suite();
    grammar::rules parse_rules();
    grammar::case_opt parse_case();
    grammar::CaseOpt parse_op();
    grammar::split_opt parse_split();
    grammar::block_opt parse_block();
    grammar::target_opt parse_target();
    
    explicit parser(lexer& lex) : tokens( lex.tokenize_source() ) {}
};



#endif
