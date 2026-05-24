#include <expected>
#include <vector>
#include <string>
#include <string_view>
#include "lexer.h"

#include <iostream>
#include <ostream>

#include "token.h"

namespace BadSQL {
    std::expected<std::vector<Token>, std::string> Lexer::tokenize() {
        std::vector<Token> tokens;

        while (std::optional<char> peek =_peek() && peek.has_value()) {
            std::cout << peek.value();
            _consume();
        }
        return std::move(tokens);
    }

    std::optional<char> Lexer::_peek(const size_t offset) const {
        if (_query.empty())
            return std::nullopt;
        if (_index + offset <= _query.size() - 1)
            return _query.at(_index + offset);
        return std::nullopt;
    }
    std::optional<char> Lexer::_peekBack(const size_t offset) const {
        if (_query.empty())
            return std::nullopt;
        if (_index < offset + 1)
            return std::nullopt;
        return _query.at(_index - offset - 1);
    }

    void Lexer::_consume(size_t amount) {
        _index += amount;
    }
} // namespace BadSQL