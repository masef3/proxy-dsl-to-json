#include "dsl/include/lexer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main() {
    // 1. Prepare sample source code directly or load from file
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

    std::cout << "--- Starting Lexer Test ---\n";

    // 2. Initialize lexer with source string
    lexer lex(source);

    // 3. Tokenize source into tokens
    lex.tokens = lex.tokenize_source();

    // 4. Print token results
    lex.print_tokens();

    std::cout << "--- Finished ---\n";

    return 0;
}
