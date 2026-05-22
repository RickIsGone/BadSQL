#include <expected>
#include <vector>
#include <string>
#include <string_view>
#include "lexer.h"

#include <iostream>
#include <ostream>

#include "token.h"

namespace BadSQL {
    std::expected<std::vector<Token>, std::string> Lexer::tokenize(std::string_view query) {
        std::vector<Token> tokens;
        for (char c : query) {
            std::cout << c;
        }
        return std::move(tokens);
    }
} // namespace BadSQL