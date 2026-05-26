#include "parser.h"
namespace BadSQL {

   std::expected<std::vector<std::unique_ptr<Expression>>, std::string> Parser::parse() const {
         std::vector<std::unique_ptr<Expression>> expressions;
         for (const auto& token : _tokens) {
            Logger::debug("Token: {} ({}:{})", token.lexeme, token.position.line, token.position.column);
         }
         return std::move(expressions);
   }
}