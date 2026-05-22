#pragma once

#include <vector>
#include <expected>
#include <string>
#include <filesystem>
#include "token.h"
namespace fs = std::filesystem;

namespace BadSQL {
    class Lexer {
    public:
        Lexer() = default;
        ~Lexer() = default;
        Lexer(const Lexer&) = delete;
        Lexer& operator=(const Lexer&) = delete;
        Lexer(Lexer&&) = delete;

        std::expected<std::vector<Token>, std::string> tokenize(const fs::path& path);

    private:
        std::string _raw{};
        size_t _index{};

    };
} // namespace BadSQL

