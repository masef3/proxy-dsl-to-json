#include "include/parser.hpp"
#include "include/lexer.hpp"
#include <vector>
#include <type_traits>


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


parser::MatchOut parser::match(type expect)
{
    if (!check( expect )) return false;

    using ConsumeOut = std::decay_t<decltype(consume(expect))>;

    try {
        consume(expect);
    }
    catch (const std::exception& error) {

        if (std::is_same_v<ConsumeOut, error_info>) {
            return parse_error{ peek().token_type, error};
        }

    }
    return true;
}








