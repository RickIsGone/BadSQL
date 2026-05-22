#pragma once

#include <vector>
#include <expected>
#include <string>
#include "token.h"

namespace BadSQL {
    class Lexer {
    public:
        Lexer() = default;
        ~Lexer() = default;
        Lexer(const Lexer&) = delete;
        Lexer& operator=(const Lexer&) = delete;
        Lexer(Lexer&&) = delete;

        std::expected<std::vector<Token>, std::string> tokenize(std::string_view query);

    private:
        size_t _index{};
    };
} // namespace BadSQL

