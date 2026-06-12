#include <expected>
#include <vector>
#include <string>
#include <string_view>
#include "lexer.hpp"

#include <ostream>

#include "token.hpp"

namespace BadSQL {
   std::expected<std::vector<Token>, std::string> Lexer::tokenize() {
      std::vector<Token> tokens;
      std::string buffer{};
      buffer.reserve(64);
      while (_peek().has_value()) {
         const std::size_t startingIndex = _index;

         if (std::isspace(static_cast<unsigned char>(_peek().value()))) {
            do {
               _consume();
               if (!_peek().has_value()) break;
            } while (_peek().has_value() && std::isspace(static_cast<unsigned char>(_peek().value())));

         } else if (std::isalpha(static_cast<unsigned char>(_peek().value())) || _peek().value() == '_') {
            // KEYWORDS AND IDENTIFIERS
            do {
               buffer.push_back(_peek().value());
               _consume();
            } while (_peek().has_value() && (std::isalnum(static_cast<unsigned char>(_peek().value())) || _peek().value() == '_'));

            std::string upperBuffer = buffer;
            for (char& c : upperBuffer) {
               c = std::toupper(static_cast<unsigned char>(c));
            }

            auto match = tryMatchToken(upperBuffer);
            tokens.push_back({match.has_value() ? match.value() : TokenType::Identifier, _position(startingIndex), buffer});

            buffer.clear();
            buffer.clear();

         } else if (std::isdigit(static_cast<unsigned char>(_peek().value()))) {
            // NUMERIC LITERALS
            do {
               buffer.push_back(_peek().value());
               _consume();
            } while (_peek().has_value() && (std::isdigit(static_cast<unsigned char>(_peek().value())) || _peek().value() == '.'));
            tokens.push_back({buffer.find('.') == std::string::npos ? TokenType::IntLiteral : TokenType::FloatLiteral, _position(startingIndex), buffer});
            buffer.clear();

         } else {
            switch (_peek().value()) {
               case '\'':
                  // STRINGS
                  _consume();

                  while (true) {
                     auto cur = _peek();
                     if (!cur.has_value()) {
                        buffer.clear();
                        return std::unexpected<std::string>("Unterminated string literal at line " + std::to_string(_position(startingIndex).line));
                     }

                     char c = cur.value();

                     if (c == '\'') {
                        if (_peek(1).has_value() && _peek(1).value() == '\'') {
                           _consume(2);
                           buffer.push_back('\'');
                           continue;
                        }

                        _consume();
                        tokens.push_back({TokenType::String, _position(startingIndex), buffer});
                        buffer.clear();
                        break;
                     }

                     if (c == '\n') {
                        buffer.clear();
                        Position pos = _position(startingIndex);
                        return std::unexpected<std::string>("Newline in string literal at line " + std::to_string(pos.line) + " col " + std::to_string(pos.column));
                     }

                     buffer.push_back(c);
                     _consume();
                  }
                  break;


               default: {
                  TokenType lastValidType = TokenType::Invalid;
                  buffer.clear();

                  for (size_t i = 0; _peek(i).has_value() && !std::isspace(static_cast<unsigned char>(_peek(i).value())) && _peek(i).value() != '\n' && !std::isalnum(static_cast<unsigned char>(_peek(i).value())) && _peek(i).value() != '_'; ++i) {
                     buffer.push_back(_peek(i).value());
                     auto opt = _determineFixedSizeTokenType(buffer);
                     if (opt.has_value() && opt.value() != TokenType::Invalid) {
                        lastValidType = opt.value();
                     } else {
                        if (buffer.length() == 1) {
                           break;
                        }
                        buffer.pop_back();
                        break;
                     }
                  }

                  if (lastValidType == TokenType::Invalid) {
                     size_t len = 0;
                     while (_peek(len).has_value() && !std::isspace(static_cast<unsigned char>(_peek(len).value())) && _peek(len).value() != '\n') {
                        ++len;
                     }
                     std::string fragment;
                     if (startingIndex < _query.size())
                        fragment = std::string(_query.substr(startingIndex, len));

                     Position pos = _position(startingIndex);
                     std::string err = "Unexpected token starting at line " + std::to_string(pos.line) + " col " + std::to_string(pos.column);
                     if (!fragment.empty()) err += ": '" + fragment + "'";
                     return std::unexpected<std::string>(err);
                  }

                  _consume(buffer.length());
                  tokens.push_back({lastValidType, _position(startingIndex), buffer});
                  buffer.clear();

               } break;
            }
         }
      }
      return std::move(tokens);
   }
   std::optional<TokenType> Lexer::_determineFixedSizeTokenType(std::string_view token) const {
      if (token.length() > 1 && token[0] != '>' && token[0] != '<' && token[0] != '!')
         return std::nullopt;

      switch (token[0]) {
         case '(':
            return TokenType::OpenParen;
         case ')':
            return TokenType::CloseParen;
         case ',':
            return TokenType::Comma;
         case ';':
            return TokenType::SemiColon;
         case '*':
            return TokenType::All;
         case '=':
            return TokenType::Equal;
         case '>':
            if (token.length() == 1)
               return TokenType::Greater;
            if (token == ">=")
               return TokenType::GreaterEqual;
            return TokenType::Invalid;
         case '<':
            if (token.length() == 1)
               return TokenType::Less;
            if (token == "<=")
               return TokenType::LessEqual;
            return TokenType::Invalid;
         case '!':
            if (token == "!=")
               return TokenType::NotEqual;
            return TokenType::Invalid;
         default:
            return TokenType::Invalid;
      }
   }
   std::optional<char> Lexer::_peek(const size_t offset) const {
      if (_query.empty())
         return std::nullopt;
      if (_index + offset <= _query.size() - 1)
         return _query.at(_index + offset);
      return std::nullopt;
   }
   std::optional<char> Lexer::_peekBack(const size_t offset) const {
      if (_query.empty())
         return std::nullopt;
      if (_index < offset + 1)
         return std::nullopt;
      return _query.at(_index - offset - 1);
   }
   Position Lexer::_position(const size_t index) const {
      size_t line = 1;
      size_t column = 1;

      for (size_t i = 0; i < index && i < _query.size(); ++i) {
         if (_query[i] == '\n') {
            ++line;
            column = 1;
         } else {
            ++column;
         }
      }

      return {line, column};
   }

   void Lexer::_consume(size_t amount) {
      _index += amount;
   }
} // namespace BadSQL