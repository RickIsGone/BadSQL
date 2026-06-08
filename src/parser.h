#pragma once

#include <vector>
#include <memory>
#include <expected>
#include <string>
#include "ast.h"
#include "token.h"
#include "types.h"
#include "logger.h"

namespace BadSQL {
   class Parser {
   public:
      explicit Parser(std::vector<Token>& tokens) : _tokens(tokens) {}
      ~Parser() = default;
      Parser(const Parser&) = delete;
      Parser& operator=(const Parser&) = delete;
      Parser(Parser&&) = delete;

      [[nodiscard]] std::expected<std::vector<std::unique_ptr<Expression>>, std::string> parse();

   private:
      [[nodiscard]] std::expected<std::unique_ptr<Expression>, std::string> _parseDDLStatement() ;
      [[nodiscard]] std::expected<std::unique_ptr<Expression>, std::string> _parseDMLStatement() ;
      [[nodiscard]] std::expected<std::unique_ptr<Expression>, std::string> _parseSelectStatement() ;
      [[nodiscard]] std::expected<std::unique_ptr<Expression>, std::string> _parseIdentifier() ;
      [[nodiscard]] std::expected<std::unique_ptr<Expression>, std::string> _parseUseStatement() ;
      [[nodiscard]] std::expected<std::unique_ptr<Expression>, std::string> _parseDatabaseStatement() ;
      [[nodiscard]] std::expected<std::unique_ptr<Expression>, std::string> _parseTableStatement() ;
      [[nodiscard]] std::expected<std::unique_ptr<Expression>, std::string> _parseTableColumn() ;
      [[nodiscard]] std::expected<std::shared_ptr<Type>, std::string> _parseType() ;

      [[nodiscard]] std::optional<Token> _peek(const size_t offset = 0) const;
      [[nodiscard]] std::optional<Token> _peekBack(const size_t offset = 0) const;
      void _consume(size_t amount = 1);

      std::vector<Token>& _tokens;
      size_t _index{0};
   };

} // namespace BadSQL
