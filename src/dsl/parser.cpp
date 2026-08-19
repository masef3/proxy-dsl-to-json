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


bool parser::match(type expect)
{
    if (!check( expect )) return false; 
    prev_token = peek();
    return tokens[ curr_pos ++ ].token_type == expect;
}


parser::ConsumeOut parser::consume(type expect, const err_decl::error_type err_t)
{
    if (match( expect )) return peek();

     
}











