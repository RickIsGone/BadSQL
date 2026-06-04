#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include "token.h"
#include "types.h"

namespace BadSQL {
   enum class ASTType {
      Create,
      Drop,

      Delete,
      Insert,
      Update,

      Use,

      Select,

      Database,
      Table,

      Identifier,

      Row
   };

   class Expression {
   public:
      Expression(Token token) : _Token(token) {}
      virtual ~Expression() = default;
      virtual ASTType kind() const noexcept = 0;

      constexpr const Token& token() const noexcept { return _Token; }

   protected:
      Token _Token;
   };

   class DDLStatement : public Expression {
   public:
      DDLStatement(Token token, ASTType type, std::unique_ptr<Expression> object)
          : Expression{token}, _kind{type}, _object{std::move(object)} {}

      ASTType kind() const noexcept override { return _kind; }
      const std::unique_ptr<Expression>& object() const { return _object; }

   private:
      ASTType _kind;
      std::unique_ptr<Expression> _object;
   };

   class DMLStatement : public Expression {
   public:
      DMLStatement(Token token, ASTType type, std::unique_ptr<Expression> table)
          : Expression{token}, _kind{type}, _table{std::move(table)} {}

      ASTType kind() const noexcept override { return _kind; }
      const std::unique_ptr<Expression>& table() const { return _table; }

   private:
      ASTType _kind;
      std::unique_ptr<Expression> _table;
   };

   class Identifier : public Expression {
   public:
      Identifier(Token token) : Expression{token} {}

      ASTType kind() const noexcept override { return ASTType::Identifier; }
      std::string_view identifier() { return _Token.lexeme; }
   };

   class Use : public Expression {
   public:
      Use(Token token, std::unique_ptr<Expression> dbName)
          : Expression{token}, _dbName{std::move(dbName)} {}

      ASTType kind() const noexcept override { return ASTType::Use; }

      const std::unique_ptr<Expression>& dbName() const { return _dbName; }
      std::unique_ptr<Expression>& dbName() { return _dbName; }

   private:
      std::unique_ptr<Expression> _dbName;
   };

   class Database : public Expression {
   public:
      Database(Token token, std::unique_ptr<Expression> name)
          : Expression{token}, _name{std::move(name)} {}

      ASTType kind() const noexcept override { return ASTType::Database; }

      const std::unique_ptr<Expression>& name() const { return _name; }
      std::unique_ptr<Expression>& name() { return _name; }

   private:
      std::unique_ptr<Expression> _name;
   };

   struct Column {
      std::unique_ptr<Identifier> name;
      std::shared_ptr<Type> type;
   };

   class Table : public Expression {
   public:
      Table(Token token, std::unique_ptr<Expression> name, std::vector<Column> table)
          : Expression{token}, _name{std::move(name)}, _table{std::move(table)} {}

      ASTType kind() const noexcept override { return ASTType::Table; }

      const std::unique_ptr<Expression>& name() const { return _name; }
      std::unique_ptr<Expression>& name() { return _name; }

      const std::vector<Column>& table() const { return _table; }
      std::vector<Column>& table() { return _table; }

   private:
      std::unique_ptr<Expression> _name;
      std::vector<Column> _table;
   };

} // namespace BadSQL
