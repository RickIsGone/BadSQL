#pragma once

#include <vector>
#include <expected>
#include <string_view>
#include <optional>
#include "token.h"

namespace BadSQL {
    class Lexer {
    public:
        Lexer(std::string_view query) : _query(query) {}
        ~Lexer() = default;
        Lexer(const Lexer&) = delete;
        Lexer& operator=(const Lexer&) = delete;
        Lexer(Lexer&&) = delete;

        [[nodiscard]] std::expected<std::vector<Token>, std::string> tokenize();

    private:
        [[nodiscard]] std::optional<char> _peek(const size_t offset = 0) const;
        [[nodiscard]] std::optional<char> _peekBack(const size_t offset = 0) const;
        void _consume(size_t amount = 1);

        size_t _index{};
        std::string_view _query;
    };
} // namespace BadSQL

