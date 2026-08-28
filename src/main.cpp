#include "dsl/include/lexer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "dsl/include/lexer.hpp"
#include "dsl/include/jsongen.hpp"
#include "nlohmann/json.hpp"

int main() {
    std::string source;
    
    std::ifstream file("dsl/examples/test1.pxy");
    if (file.is_open()) {
        std::stringstream ss;
        ss << file.rdbuf();
        source = ss.str();
    } else {
        // Fallback string if the file doesn't exist
        source = "suite port = rules case split target value percent pipe ; : ( )";
    }

    lexer lex(source);
    parser pars(lex);
    grammar::program program_ast = pars.parse_program();
    export_to_json(program_ast, "jsongen.json");

    return 0;
}
