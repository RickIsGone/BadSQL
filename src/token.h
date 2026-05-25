#pragma once

#include <unordered_map>
#include <string_view>
#include <optional>

namespace BadSQL {
   enum class TokenType {
      Invalid,

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
      IntLiteral,
      FloatLiteral,
      String,

      OpenParen,
      CloseParen,

      Comma,
      SemiColon,

      I32,
      U32,
      F32,
      F64,
      Varchar,
      Char,

      Greater,
      GreaterEqual,
      Less,
      LessEqual,
      Equal,
      NotEqual
   };

   struct Position {
      size_t line;
      size_t column;
   };

   struct Token {
      TokenType type;
      Position position;
      std::string lexeme;
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
       {"f32", TokenType::F32},
       {"f64", TokenType::F64},
       {"varchar", TokenType::Varchar},
       {"char", TokenType::Char}};

   inline std::optional<TokenType> tryMatchToken(std::string_view token) {
      if (tokenMap.contains(token))
         return tokenMap.at(token);

      return std::nullopt;
   }
} // namespace BadSQL