#include "dsl/include/lexer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "dsl/include/lexer.hpp"
#include "dsl/include/jsongen.hpp"
#include "nlohmann/json.hpp"
#include <span>

int main(int argc, char** argv) {
    std::span<char *> args(argv, argc);
    if (args.size() < 2 || args.size() > 2) {
        std::cerr << "Usage: " << args[0] << " <path_to_file>\n";
        return 1;
    }

    std::string fp = args[1];
    auto extension = fp.find(".tjs");
    if (extension == std::string::npos) {
        std::cerr << "Given file does not have .tjs extension\n";
    }
    
    std::ifstream file(fp);
    std::stringstream ss;
    ss << file.rdbuf();
    std::string source = ss.str();

    lexer lex(source);
    parser pars(lex);

    lex.print_tokens();
    grammar::program program_ast = pars.parse_program();
    export_to_json(program_ast, "jsongen.json");

    return 0;
}
