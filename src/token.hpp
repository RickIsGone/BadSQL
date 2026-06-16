#pragma once

#include <unordered_map>
#include <string_view>
#include <optional>

/** @file
 * @brief Token definitions and utilities for the BadSQL lexer
 * Defines TokenType enum, Token struct, and token matching utilities.
 */

namespace BadSQL {
   /** @brief Enum class representing all possible token types in SQL
    *
    * Includes keywords, operators, literals, and special characters.
    */
   enum class TokenType {
      Invalid, ///< Invalid or unknown token

      // DDL keywords
      Create, ///< CREATE keyword
      Drop,   ///< DROP keyword

      // DML keywords
      Delete, ///< DELETE keyword
      Insert, ///< INSERT keyword
      Update, ///< UPDATE keyword

      // Query keywords
      Select, ///< SELECT keyword

      // Object types
      Database, ///< DATABASE keyword
      Table,    ///< TABLE keyword

      // Statement keywords
      Use, ///< USE keyword

      // Clauses
      From,   ///< FROM keyword
      Where,  ///< WHERE keyword
      Into,   ///< INTO keyword
      Values, ///< VALUES keyword
      All,    ///< * (all columns)

      // Literals and identifiers
      Identifier,   ///< User-defined identifier
      IntLiteral,   ///< Integer literal (e.g., 42)
      FloatLiteral, ///< Floating point literal (e.g., 3.14)
      String,       ///< String literal (e.g., 'hello')

      // Delimiters
      OpenParen,  ///< ( (opening parenthesis)
      CloseParen, ///< ) (closing parenthesis)

      Comma,      ///< , (comma)
      SemiColon,  ///< ; (semicolon)

      // Data types
      I32,     ///< i32 type
      U32,     ///< u32 type
      F32,     ///< f32 type
      F64,     ///< f64 type
      Varchar, ///< varchar type
      Char,    ///< char type

      // Operators
      Greater,      ///< > (greater than)
      GreaterEqual, ///< >= (greater than or equal)
      Less,         ///< < (less than)
      LessEqual,    ///< <= (less than or equal)
      Equal,        ///< = (equal)
      NotEqual      ///< != (not equal)
   };

   /** @brief Represents a position in the source SQL query
    *
    * Stores line and column information for error reporting.
    */
   struct Position {
      size_t line;   ///< Line number (1-based)
      size_t column; ///< Column number (1-based)

      /** @brief Convert position to string representation
       * @return String in format "line:column"
       */
      [[nodiscard]] std::string string() const {
         return std::to_string(line) + ":" + std::to_string(column);
      }
   };

   /** @brief Represents a single token from the lexer
    *
    * Contains the token type, position in source, and the lexeme.
    */
   struct Token {
      TokenType type;     ///< The type of the token
      Position position;  ///< The position in the source query
      std::string lexeme; ///< The actual text of the token
   };

   /** @brief Map of keyword strings to token types
    *
    * Used by the lexer to identify SQL keywords.
    */
   static std::unordered_map<std::string_view, TokenType> tokenMap = {
       {"CREATE", TokenType::Create},
       {"DROP", TokenType::Drop},
       {"DELETE", TokenType::Delete},
       {"INSERT", TokenType::Insert},
       {"UPDATE", TokenType::Update},

       {"SELECT", TokenType::Select},

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

   /** @brief Try to match a string to a token type
    *
    * Searches the token map for the given string.
    * Used by the lexer to identify keywords.
    *
    * @param token The string to match (case-sensitive)
    * @return Optional containing the TokenType if found, nullopt otherwise
    */
   inline std::optional<TokenType> tryMatchToken(std::string_view token) {
      if (tokenMap.contains(token))
         return tokenMap.at(token);

      return std::nullopt;
   }
} // namespace BadSQL