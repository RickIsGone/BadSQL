#pragma once

#include <unordered_map>
#include <string_view>
#include <optional>

namespace BadSQL {
    enum class TokenType {
        Create,
        Drop,
        Delete,
        Insert,
        Update,

        Database,
        Table,

        Use,

        From,
        Where,
        Into,
        Values,
        All, // *

        Identifier,
        Number,
        String,

        OpenParen,
        CloseParen,

        Comma,
        SemiColon,

        I32,
        U32,
        Float,
        Double,
        Varchar,
        Char
    };

    struct Position {
        size_t line;
        size_t column;
    };

    struct Token {
        TokenType type;
        Position position;
    };

    static std::unordered_map<std::string_view, TokenType> tokenMap = {
        {"CREATE", TokenType::Create},
        {"DROP", TokenType::Drop},
        {"DELETE", TokenType::Delete},
        {"INSERT", TokenType::Insert},
        {"UPDATE", TokenType::Update},

        {"DATABASE", TokenType::Database},
        {"TABLE", TokenType::Table},

        {"USE", TokenType::Use},

        {"FROM", TokenType::From},
        {"WHERE", TokenType::Where},
        {"INTO", TokenType::Into},
        {"VALUES", TokenType::Values},

        {"i32", TokenType::I32},
        {"u32", TokenType::U32},
        {"float", TokenType::Float},
        {"double", TokenType::Double},
        {"varchar", TokenType::Varchar},
        {"char", TokenType::Char}
    };

    inline std::optional<TokenType> tryMatchToken(std::string_view token) {
        if (tokenMap.contains(token))
            return tokenMap.at(token);

        return std::nullopt;
    }
}