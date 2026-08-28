#include "include/parser.hpp"
#include "include/lexer.hpp"
#include <vector>
#include <type_traits>
#include <algorithm>
#include <iostream>
#include <variant>
#include <print>
#include <utility>


token parser::offset_peek(size_t offset) const
{
    if ((curr_pos + offset >= get_tokens_size()) || (curr_pos + offset < 0)) return {};
    return tokens[curr_pos + offset];

}


token parser::get_prev() const
{
    if (curr_pos == 0) throw;
    return prev_token;
}


const size_t parser::get_tokens_size() const
{
    return tokens.size();
}


token parser::get_next() const
{
    if (curr_pos == get_tokens_size() - 1) throw;
    return offset_peek(1);
}


bool parser::check_error(ConsumeOut& out) const
{
    if (std::holds_alternative<err_decl::parse_error>(out)) {
        auto& err = std::get<err_decl::parse_error>(out);
        std::println("{} : {}", token_to_str(err.token_t), err.descr);
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
    return tokens[ curr_pos ++ ].token_type == expect; // TODO fix encapsulation
}


parser::ConsumeOut parser::consume(type expect, const err_decl::error_type err_t)
{
    if (match( expect )) return peek();
    
    return err_decl::parse_error{ expect, get_error_type_message(err_t) };
}


std::optional<int> parser::parse_port()
{
    if ( peek().token_type != type::TOKEN_PORT ) return std::nullopt;

    auto out = consume(type::TOKEN_PORT, err_decl::error_type::INVALID_TOKEN);
    if (check_error(out)) return std::nullopt;

    out = consume(type::TOKEN_EQUALS, err_decl::error_type::SYNTAX_ERROR);
    if (check_error(out)) return std::nullopt;

    const token& tok = peek();
    if (std::all_of(tok.lexeme.begin(), tok.lexeme.end(), ::isdigit)) {
        out = consume(type::TOKEN_VALUE, err_decl::error_type::INVALID_VALUE);
        if (check_error(out)) return std::nullopt;
        return std::stoi(tok.lexeme);
    }

    return std::nullopt;
}


grammar::target_opt parser::parse_target()
{
    auto out = consume(type::TOKEN_TARGET, err_decl::error_type::INVALID_TOKEN);
    if (check_error(out)) return {};

    out = consume(type::TOKEN_STRING, err_decl::error_type::INVALID_STRING);
    if (check_error(out)) return {};

    return grammar::target_opt{ std::move(get_prev().lexeme) };
}


grammar::block_opt parser::parse_block()
{
    auto out = consume(type::TOKEN_BLOCK, err_decl::error_type::INVALID_TOKEN);
    if (check_error(out)) return {};
    std::vector<grammar::StrOpt> args{};

    while (peek().token_type == type::TOKEN_STRING) {
        out = consume(type::TOKEN_STRING, err_decl::error_type::INVALID_STRING);
        if (check_error(out)) return {};

        args.emplace_back(get_prev().lexeme);
    }

    return grammar::block_opt{ args };
}


grammar::split_arg parser::parse_split_arg(bool& trigger)
{
    auto out = consume(type::TOKEN_STRING, err_decl::error_type::INVALID_STRING);
    if (check_error(out)) {
        trigger = true;
        return {};
    }

    std::string url = std::move(get_prev().lexeme);

    out = consume(type::TOKEN_EQUALS, err_decl::error_type::SYNTAX_ERROR);
    if (check_error(out)) {
        trigger = true;
        return {};
    }

    out = consume(type::TOKEN_VALUE, err_decl::error_type::INVALID_VALUE);
    if (check_error(out)) {
        trigger = true;
        return {};
    }

    // sorry for redundancy :(

    return grammar::split_arg{ std::move(url), std::stoi(get_prev().lexeme) };
}


grammar::split_opt parser::parse_split()
{
    auto out = consume(type::TOKEN_SPLIT, err_decl::error_type::INVALID_TOKEN);
    if (check_error(out)) return {};
    std::vector<grammar::split_arg> args{};
    bool trigger = false;

    while (peek().token_type == type::TOKEN_STRING && !trigger) {
        grammar::split_arg arg = parse_split_arg(trigger);
        if (!trigger) {
            args.push_back(std::move(arg));
        }
    }
    
    if (trigger) return {};
    return grammar::split_opt{ std::move(args) };
}


grammar::case_opt parser::parse_op()
{
    auto out = consume(type::TOKEN_CASE, err_decl::error_type::INVALID_TOKEN);
    if (check_error(out)) return {};

    out = consume(type::TOKEN_STRING, err_decl::error_type::INVALID_STRING);
    if (check_error(out)) return {};
    std::string opt_name = get_prev().lexeme;

    out = consume(type::TOKEN_LBRACE, err_decl::error_type::MISSING_TOKEN);
    if (check_error(out)) return {};

    auto choose_opt = [&]()
    {
        std::string curr = peek().lexeme;
        auto cmapped = (std::find_if(cmap.begin(), cmap.end(), [curr](const case_opt_mapping& copt) {
            return curr == copt.token_name;
        }));

        if (cmapped == cmap.end()) throw;
        
        grammar::CaseOpt ret_op = cmapped->f(*this);
        return ret_op;
    };


    using retType = decltype(std::declval<case_opt_mapping>().f(std::declval<parser&>()));
    retType ret_res = choose_opt();

    out = consume(type::TOKEN_RBRACE, err_decl::error_type::MISSING_TOKEN);
    if (check_error(out)) return {};

    return grammar::case_opt{ std::move(opt_name), std::move(ret_res) };
}


grammar::rules parser::parse_rules()
{
    auto out = consume(type::TOKEN_RULES, err_decl::error_type::INVALID_TOKEN);
    if (check_error(out)) return {};

    out = consume(type::TOKEN_LBRACE, err_decl::error_type::MISSING_TOKEN);
    if (check_error(out)) return {};

    grammar::RuleOpt args{};

    while (peek().token_type == type::TOKEN_CASE) { // add error handling.
        args.push_back(parse_op());
    }

    out = consume(type::TOKEN_RBRACE, err_decl::error_type::MISSING_TOKEN);
    if (check_error(out)) return {};

    return grammar::rules{ args };
}


grammar::suite parser::parse_suite()
{
    auto out = consume(type::TOKEN_SUITE, err_decl::error_type::INVALID_TOKEN);
    if (check_error(out)) return {};

    out = consume(type::TOKEN_LBRACE, err_decl::error_type::MISSING_TOKEN);
    if (check_error(out)) return {};

    std::optional<int> port = parse_port();
    grammar::rules args = parse_rules();

    out = consume(type::TOKEN_RBRACE, err_decl::error_type::MISSING_TOKEN);
    if (check_error(out)) return {};

    using portTypeRes = std::decay_t<decltype(port)>;
    if (std::is_same_v<portTypeRes, int>) return grammar::suite{ port, args };

    return grammar::suite{ args };
}


grammar::program parser::parse_program()
{
    std::vector<grammar::suite> suites{};

    while (peek().token_type == type::TOKEN_SUITE) {
        suites.push_back(parse_suite());
    }

    return grammar::program{ suites };
}

namespace grammar {

    using json = nlohmann::json;
    void to_json(json& output, const case_opt& curr_case)
    {
        output["case"] = curr_case.case_target_name;
        std::visit([&output] (const auto& curr_case_value)
                {
                    output.update(json(curr_case_value));
                },
                curr_case.case_args);
    }


    void to_json(json& output, const grammar::split_opt& arg)
    {
        output = json{
            {"function", "split"},
            {"targets", arg.split_args}
        };
    }


    void to_json(json& output, const grammar::block_opt& arg)
    {
        output = json{
            {"function", "block"},
            {"targets", arg.block_args}
        };
    }


    void to_json(json& output, const grammar::target_opt& arg)
    {
        output = json{
            {"function", "target"},
            {"targets", arg.target_args}
        };
    }


    void to_json(json& output, const grammar::rules& arg)
    {
        output = arg.rules_args;
    }


    void to_json(json& output, const grammar::suite& arg)
    {
        output["rules"] = arg.rules_;
    }

}
