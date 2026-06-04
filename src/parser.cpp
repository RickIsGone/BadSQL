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
               Logger::error("{}:{} Unexpected token: {}", token.position.column, token.position.line, token.lexeme);
               _consume();
            } break;
         }
      }
      return std::move(expressions);
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