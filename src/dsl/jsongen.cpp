#include "include/jsongen.hpp"
#include "include/parser.hpp"
#include "include/lexer.hpp"
#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <string_view>

void export_to_json(grammar::program& program, std::string_view filename)
{
    json program_out = program;
    std::ofstream out(filename.data());

    if (out.is_open()) {
        out << program_out.dump(2);
        out.close();
    }
}
