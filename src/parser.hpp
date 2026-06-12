#pragma once

#include <vector>
#include <memory>
#include <expected>
#include <string>
#include "ast.hpp"
#include "token.hpp"
#include "types.hpp"
#include "logger.hpp"
/** @file
 * @brief @ref Parser Class definitions
 * @details Contains the definition of the @ref Parser class
 */
namespace BadSQL {
   /** @brief Parser for the SQL grammar supported by BadSQL
    *
    * Converts a token sequence produced by the lexer into a collection of
    * higher-level AST nodes. The parser currently recognizes DDL statements,
    * some DML statements, `USE`, and `SELECT`.
    */
   class Parser {
   public:
      /** @brief Construct a parser from a token sequence
       * @param tokens Reference to the token list to analyze
       */
      explicit Parser(std::vector<Token>& tokens) : _tokens(tokens) {}

      /**
       * @brief Destructor
       */
      ~Parser() = default;

      // Disable copy and move operations
      Parser(const Parser&) = delete;
      Parser& operator=(const Parser&) = delete;
      Parser(Parser&&) = delete;

      /** @brief Analyze tokenized input and produce statement ASTs
       *
       * Scans the token sequence, recognizes supported statements, and builds
       * a vector of AST expressions. Parsing errors are reported through
       * `std::expected` and, where appropriate, through the logger as well.
       *
       * @return A `std::vector` of `std::unique_ptr<Expression>` on success,
       *         otherwise an error message
       */
      [[nodiscard]] std::expected<std::vector<std::unique_ptr<Expression>>, std::string> parse();

   private:
      /** @brief Parse a DDL statement (`CREATE` / `DROP`)
       * @return DDL AST node or an error message
       */
      [[nodiscard]] std::expected<std::unique_ptr<Expression>, std::string> _parseDDLStatement();

      /** @brief Parse a DML statement (`INSERT` / `UPDATE` / `DELETE`)
       * @return DML AST node or an error message
       */
      [[nodiscard]] static std::expected<std::unique_ptr<Expression>, std::string> _parseDMLStatement();

      /** @brief Parse a `SELECT` statement
       * @return SELECT AST node or an error message
       */
      [[nodiscard]] static std::expected<std::unique_ptr<Expression>, std::string> _parseSelectStatement();

      /** @brief Parse an SQL identifier
       * @return `Identifier` AST node or an error message
       */
      [[nodiscard]] std::expected<std::unique_ptr<Expression>, std::string> _parseIdentifier();

      /** @brief Parse a `USE` statement
       * @return `USE` AST node or an error message
       */
      [[nodiscard]] static std::expected<std::unique_ptr<Expression>, std::string> _parseUseStatement();

      /** @brief Parse a `DATABASE` node
       * @return `Database` AST node or an error message
       */
      [[nodiscard]] std::expected<std::unique_ptr<Expression>, std::string> _parseDatabaseStatement();

      /** @brief Parse a `TABLE` node
       * @return `Table` AST node or an error message
       */
      [[nodiscard]] static std::expected<std::unique_ptr<Expression>, std::string> _parseTableStatement();

      /** @brief Parse a table column definition
       * @return Column AST node or an error message
       */
      [[nodiscard]] static std::expected<std::unique_ptr<Expression>, std::string> _parseTableColumn();

      /** @brief Parse a supported SQL type
       * @return Shared type instance or an error message
       */
      [[nodiscard]] static std::expected<std::shared_ptr<Type>, std::string> _parseType();

      /** @brief Return the current token without consuming it
       * @param offset Offset from the current token (default 0)
       * @return Optional token at the requested position
       */
      [[nodiscard]] std::optional<Token> _peek(size_t offset = 0) const;

      /** @brief Return a previous token without consuming it
       * @param offset Backward offset from the current token (default 0)
       * @return Optional token before the current position
       */
      [[nodiscard]] std::optional<Token> _peekBack(size_t offset = 0) const;

      /** @brief Advance the parser's internal index
       * @param amount Number of tokens to consume
       */
      void _consume(size_t amount = 1);

      std::vector<Token>& _tokens;
      size_t _index{0};
   };

} // namespace BadSQL
