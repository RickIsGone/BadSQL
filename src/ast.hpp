#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include "token.hpp"
#include "types.hpp"

/** @file
 * @brief Abstract Syntax Tree (AST) definitions for BadSQL
 * Contains base expression classes and AST node types for SQL statements.
 */

namespace BadSQL {
   /** @brief Enum class representing different types of AST nodes
    *
    * Defines the kind of SQL statement or expression represented by an AST node.
    */
   enum class ASTType {
      Create,     ///< CREATE statement
      Drop,       ///< DROP statement
      Delete,     ///< DELETE statement
      Insert,     ///< INSERT statement
      Update,     ///< UPDATE statement
      Use,        ///< USE statement
      Select,     ///< SELECT statement
      Database,   ///< DATABASE node
      Table,      ///< TABLE node
      Identifier, ///< IDENTIFIER node
      Row         ///< ROW node
   };

   /** @brief Base class for all AST expressions
    *
    * Abstract base class that all expression nodes in the AST inherit from.
    * Provides common functionality for token storage and type identification.
    */
   class Expression {
   public:
      /** @brief Construct an Expression with a token
       * @param token The token associated with this expression
       */
      Expression(Token token) : _Token(token) {}

      /** @brief Virtual destructor */
      virtual ~Expression() = default;

      /** @brief Get the AST node type
       * @return The ASTType of this expression
       */
      [[nodiscard]] virtual ASTType kind() const noexcept = 0;

      /** @brief Get the token associated with this expression
       * @return Const reference to the token
       */
      [[nodiscard]] constexpr const Token& token() const noexcept { return _Token; }

   protected:
      Token _Token; ///< The token associated with this expression
   };

   /** @brief Represents a DDL (Data Definition Language) statement
    *
    * Handles CREATE TABLE/DATABASE and DROP TABLE/DATABASE statements.
    */
   class DDLStatement : public Expression {
   public:
      /** @brief Construct a DDL statement
       * @param token The token representing the statement
       * @param type The type of DDL statement (Create or Drop)
       * @param object The database or table object being created/dropped
       */
      DDLStatement(Token token, ASTType type, std::unique_ptr<Expression> object)
          : Expression{token}, _kind{type}, _object{std::move(object)} {}

      /** @brief Get the kind of DDL statement
       * @return ASTType::Create or ASTType::Drop
       */
      ASTType kind() const noexcept override { return _kind; }

      /** @brief Get the object being created/dropped
       * @return Const reference to the expression (Database or Table)
       */
      const std::unique_ptr<Expression>& object() const { return _object; }

   private:
      ASTType _kind;                       ///< The kind of DDL statement
      std::unique_ptr<Expression> _object; ///< The object (Database or Table)
   };

   /** @brief Represents a DML (Data Manipulation Language) statement
    *
    * Handles INSERT, UPDATE, and DELETE statements.
    */
   class DMLStatement : public Expression {
   public:
      /** @brief Construct a DML statement
       * @param token The token representing the statement
       * @param type The type of DML statement (Insert, Update, or Delete)
       * @param table The table being manipulated
       */
      DMLStatement(Token token, ASTType type, std::unique_ptr<Expression> table)
          : Expression{token}, _kind{type}, _table{std::move(table)} {}

      /** @brief Get the kind of DML statement
       * @return ASTType::Insert, Update, or Delete
       */
      ASTType kind() const noexcept override { return _kind; }

      /** @brief Get the table being manipulated
       * @return Const reference to the table expression
       */
      const std::unique_ptr<Expression>& table() const { return _table; }

   private:
      ASTType _kind;                      ///< The kind of DML statement
      std::unique_ptr<Expression> _table; ///< The table being manipulated
   };

   /** @brief Represents an identifier in SQL
    *
    * An identifier is typically a name for a table, database, or column.
    */
   class Identifier : public Expression {
   public:
      /** @brief Construct an identifier
       * @param token The token representing the identifier
       */
      Identifier(Token token) : Expression{token} {}

      /** @brief Get the kind of expression
       * @return ASTType::Identifier
       */
      ASTType kind() const noexcept override { return ASTType::Identifier; }

      /** @brief Get the identifier name
       * @return String view of the identifier lexeme
       */
      std::string_view identifier() { return _Token.lexeme; }
   };

   /** @brief Represents a USE statement
    *
    * Selects which database to use for subsequent operations.
    */
   class Use : public Expression {
   public:
      /** @brief Construct a USE statement
       * @param token The token representing the USE keyword
       * @param dbName The name of the database to use
       */
      Use(Token token, std::unique_ptr<Expression> dbName)
          : Expression{token}, _dbName{std::move(dbName)} {}

      /** @brief Get the kind of expression
       * @return ASTType::Use
       */
      ASTType kind() const noexcept override { return ASTType::Use; }

      /** @brief Get the database name (const reference)
       * @return Const reference to the database name expression
       */
      const std::unique_ptr<Expression>& dbName() const { return _dbName; }

      /** @brief Get the database name (mutable reference)
       * @return Mutable reference to the database name expression
       */
      std::unique_ptr<Expression>& dbName() { return _dbName; }

   private:
      std::unique_ptr<Expression> _dbName; ///< The name of the database to use
   };

   /** @brief Represents a database in the AST
    *
    * Used in CREATE DATABASE and DROP DATABASE statements.
    */
   class Database : public Expression {
   public:
      /** @brief Construct a Database node
       * @param token The token representing the DATABASE keyword
       * @param name The name of the database
       */
      Database(Token token, std::unique_ptr<Expression> name)
          : Expression{token}, _name{std::move(name)} {}

      /** @brief Get the kind of expression
       * @return ASTType::Database
       */
      ASTType kind() const noexcept override { return ASTType::Database; }

      /** @brief Get the database name (const reference)
       * @return Const reference to the database name expression
       */
      const std::unique_ptr<Expression>& name() const { return _name; }

      /** @brief Get the database name (mutable reference)
       * @return Mutable reference to the database name expression
       */
      std::unique_ptr<Expression>& name() { return _name; }

   private:
      std::unique_ptr<Expression> _name; ///< The name of the database
   };

   /** @brief Represents a column definition in a table
    *
    * Contains the column name and type information.
    */
   struct Column {
      std::unique_ptr<Identifier> name; ///< The column name
      std::shared_ptr<Type> type;       ///< The column type
   };

   /** @brief Represents a table in the AST
    *
    * Used in CREATE TABLE and DROP TABLE statements.
    * Contains column definitions for CREATE operations.
    */
   class Table : public Expression {
   public:
      /** @brief Construct a Table node
       * @param token The token representing the TABLE keyword
       * @param name The name of the table
       * @param table Vector of column definitions
       */
      Table(Token token, std::unique_ptr<Expression> name, std::vector<Column> table)
          : Expression{token}, _name{std::move(name)}, _table{std::move(table)} {}

      /** @brief Get the kind of expression
       * @return ASTType::Table
       */
      ASTType kind() const noexcept override { return ASTType::Table; }

      /** @brief Get the table name (const reference)
       * @return Const reference to the table name expression
       */
      const std::unique_ptr<Expression>& name() const { return _name; }

      /** @brief Get the table name (mutable reference)
       * @return Mutable reference to the table name expression
       */
      std::unique_ptr<Expression>& name() { return _name; }

      /** @brief Get the columns (const reference)
       * @return Const reference to the column vector
       */
      const std::vector<Column>& table() const { return _table; }

      /** @brief Get the columns (mutable reference)
       * @return Mutable reference to the column vector
       */
      std::vector<Column>& table() { return _table; }

   private:
      std::unique_ptr<Expression> _name; ///< The name of the table
      std::vector<Column> _table;        ///< The columns in the table
   };

} // namespace BadSQL
