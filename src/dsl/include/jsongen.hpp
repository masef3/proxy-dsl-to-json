#ifndef JSONGEN_H
#define JSONGEN_H
#include "parser.hpp"
#include "lexer.hpp"
#include "nlohmann/json.hpp"
#include <vector>
#include <string_view>

using json = nlohmann::json;
void export_to_json(grammar::program& program, std::string_view filename);


#endif
