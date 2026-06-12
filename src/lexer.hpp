#pragma once

#include <vector>
#include <expected>
#include <string_view>
#include <optional>
#include "token.hpp"

/** @file
 * @brief Lexer for tokenizing SQL queries
 * Converts raw SQL strings into a stream of tokens for the parser.
 */

namespace BadSQL {
   /** @brief Lexer class for SQL tokenization
    *
    * Scans a SQL query string character by character and produces a vector of tokens.
    * Handles keywords, identifiers, operators, literals, and whitespace.
    */
   class Lexer {
   public:
      /** @brief Construct a Lexer with a SQL query string
       * @param query The SQL query to tokenize (stored as string_view)
       */
      explicit Lexer(std::string_view query) : _query(query) {}

      /** @brief Destructor */
      ~Lexer() = default;

      // Disable copy and move operations
      Lexer(const Lexer&) = delete;
      Lexer& operator=(const Lexer&) = delete;
      Lexer(Lexer&&) = delete;

      /** @brief Tokenize the query string
       *
       * Scans through the entire query and produces a vector of tokens.
       * Returns an error if the query contains invalid syntax.
       *
       * @return std::expected containing either the token vector or an error message
       */
      [[nodiscard]] std::expected<std::vector<Token>, std::string> tokenize();

   private:
      /** @brief Peek at a character without consuming it
       * @param offset Offset from current position (default 0 = current character)
       * @return Optional containing the character, or nullopt if at end
       */
      [[nodiscard]] std::optional<char> _peek(const size_t offset = 0) const;

      /** @brief Peek at a previous character
       * @param offset Offset backwards from current position
       * @return Optional containing the character, or nullopt if before start
       */
      [[nodiscard]] std::optional<char> _peekBack(const size_t offset = 0) const;

      /** @brief Determine the token type for operator/delimiter sequences
       *
       * Matches fixed-size tokens like operators (==, >=, <=, !=, etc.)
       * and single-character delimiters.
       *
       * @param token The string to match
       * @return Optional containing the TokenType, or Invalid if no match
       */
      [[nodiscard]] std::optional<TokenType> _determineFixedSizeTokenType(std::string_view token) const;

      /** @brief Get the line and column position for a given index
       * @param index Position in the query string
       * @return Position struct with line and column information
       */
      [[nodiscard]] Position _position(const size_t index) const;

      /** @brief Consume characters from the current position
       * @param amount Number of characters to consume (default 1)
       */
      void _consume(size_t amount = 1);

      size_t _index{0};              ///< Current position in the query
      std::string_view _query;       ///< The SQL query being tokenized
   };
} // namespace BadSQL
