#include "parser.h"

namespace BadSQL {
   std::expected<std::vector<std::unique_ptr<Expression>>, std::string> Parser::parse() {
      std::vector<std::unique_ptr<Expression>> expressions;
      while (_peek().has_value()) {
         auto token = _peek().value();
         switch (token.type) {
            case TokenType::Create:
            case TokenType::Drop: {
               auto res = _parseDDLStatement();
               if (!res)
                  Logger::error(res.error());
               else
                  expressions.push_back(std::move(res.value()));
            } break;

            case TokenType::Delete:
            case TokenType::Insert:
            case TokenType::Update: {
               auto res = _parseDMLStatement();
               if (!res)
                  Logger::error(res.error());
               else
                  expressions.push_back(std::move(res.value()));
            } break;

            case TokenType::Use: {
               auto res = _parseUseStatement();
               if (!res)
                  Logger::error(res.error());
               else
                  expressions.push_back(std::move(res.value()));
            } break;

            case TokenType::Select: {
               auto res = _parseSelectStatement();
               if (!res)
                  Logger::error(res.error());
               else
                  expressions.push_back(std::move(res.value()));
            } break;

            default: {
               Logger::error("{} Unexpected token: {}", token.position.string(), token.lexeme);
               _consume();
            } break;
         }

         if (_peek().has_value() && _peek().value().type != TokenType::SemiColon)
            Logger::error("Expected ';' at " + _peek().value().position.string());
         _consume();
      }
      return std::move(expressions);
   }

   std::expected<std::unique_ptr<Expression>, std::string> Parser::_parseDDLStatement() {
      Token tok = _peek().value();
      ASTType type = tok.type == TokenType::Create ? ASTType::Create : ASTType::Drop;
      _consume();

      if (!_peek().has_value())
         return std::unexpected("Expected 'TABLE' or 'DATABASE' after '" + tok.lexeme + "' at " + tok.position.string());

      if (_peek().value().type != TokenType::Table && _peek().value().type != TokenType::Database)
         return std::unexpected("Expected 'TABLE' or 'DATABASE' after '" + tok.lexeme + "' at " + tok.position.string() + ", got '" + _peek().value().lexeme + "' instead");

      auto exprRes = _peek().value().type == TokenType::Table ? _parseTableStatement() : _parseDatabaseStatement();
      if (!exprRes)
         return std::unexpected(exprRes.error());

      return std::make_unique<DDLStatement>(tok, type, std::move(exprRes.value()));
   }

   std::expected<std::unique_ptr<Expression>, std::string> Parser::_parseDatabaseStatement() {
      Token tok = _peek().value();
      _consume();

      if (!_peek().has_value())
         return std::unexpected("Expected an identifier after 'DATABASE' at " + tok.position.string());

      auto idRes = _parseIdentifier();
      if (!idRes)
         return std::unexpected(idRes.error());

      return std::make_unique<Database>(tok, std::move(idRes.value()));
   }

   std::expected<std::unique_ptr<Expression>, std::string> Parser::_parseDMLStatement() { return nullptr; }
   std::expected<std::unique_ptr<Expression>, std::string> Parser::_parseSelectStatement() { return nullptr; }
   std::expected<std::unique_ptr<Expression>, std::string> Parser::_parseUseStatement() { return nullptr; }
   std::expected<std::unique_ptr<Expression>, std::string> Parser::_parseTableStatement() { return nullptr; }
   std::expected<std::unique_ptr<Expression>, std::string> Parser::_parseTableColumn() { return nullptr; }
   std::expected<std::shared_ptr<Type>, std::string> Parser::_parseType() { return nullptr; }

   std::expected<std::unique_ptr<Expression>, std::string> Parser::_parseIdentifier() {
      Token tok = _peek().value();
      if (tok.type != TokenType::Identifier)
         return std::unexpected("Expected an identifier at " + tok.position.string() + ", got '" + tok.lexeme + "' instead");
      _consume();
      return std::make_unique<Identifier>(tok);
   }

   std::optional<Token> Parser::_peek(const size_t offset) const {
      if (_tokens.empty())
         return std::nullopt;
      if (_index + offset <= _tokens.size() - 1)
         return _tokens.at(_index + offset);
      return std::nullopt;
   }
   std::optional<Token> Parser::_peekBack(const size_t offset) const {
      if (_tokens.empty())
         return std::nullopt;
      if (_index < offset + 1)
         return std::nullopt;
      return _tokens.at(_index - offset - 1);
   }
   void Parser::_consume(size_t amount) {
      _index += amount;
   }
} // namespace BadSQL