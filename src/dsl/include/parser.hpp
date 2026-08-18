#include <vector>
#include <variant>
#include <optional>
#include <string>

// camelCase -> types
// snake_case -> structs


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
struct rules_ { RuleOpt args; };


struct suite
{
    std::optional<int> port{};
    const rules_ rules{};
};
