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

      [[nodiscard]] std::expected<std::vector<std::unique_ptr<Expression>>, std::string> parse() const;

   private:
      std::vector<Token>& _tokens;
   };

} // namespace BadSQL
