#pragma once

#include <memory>
#include "token.hpp"

/** @file
 * @brief Type system definitions for BadSQL
 * Defines all SQL data types (I32, U32, F32, F64, Varchar, Char) and their management.
 */

namespace BadSQL {
   /** @brief Enum class representing SQL data types
    *
    * Defines all primitive and string types supported by BadSQL.
    */
   enum class TypeKind {
      I32,     ///< 32-bit signed integer
      U32,     ///< 32-bit unsigned integer
      F32,     ///< 32-bit floating point
      F64,     ///< 64-bit floating point
      Varchar, ///< Variable-length string
      Char     ///< Fixed-length character
   };

   /** @brief Base class for all SQL types
    *
    * Abstract base class for type system. All concrete types inherit from this.
    */
   class Type {
   public:
      /** @brief Construct a Type with a token
       * @param token The token representing the type
       */
      explicit Type(Token token) : _Token{token} {}

      /** @brief Virtual destructor */
      virtual ~Type() = default;

      /** @brief Get the kind of type
       * @return The TypeKind of this type
       */
      virtual TypeKind kind() = 0;

      /** @brief Get the token associated with this type
       * @return Const reference to the token
       */
      [[nodiscard]] constexpr const Token& token() const noexcept { return _Token; }

   protected:
      Token _Token; ///< The token representing this type
   };

   /** @brief 32-bit signed integer type
    *
    * Represents the I32 SQL data type.
    */
   class I32 : public Type {
   public:
      /** @brief Construct an I32 type
       * @param token The token representing I32
       */
      explicit I32(Token token) : Type{token} {}

      /** @brief Get the kind of type
       * @return TypeKind::I32
       */
      TypeKind kind() override { return TypeKind::I32; }
   };

   /** @brief 32-bit unsigned integer type
    *
    * Represents the U32 SQL data type.
    */
   class U32 : public Type {
   public:
      /** @brief Construct a U32 type
       * @param token The token representing U32
       */
      explicit U32(Token token) : Type{token} {}

      /** @brief Get the kind of type
       * @return TypeKind::U32
       */
      TypeKind kind() override { return TypeKind::U32; }
   };

   /** @brief 32-bit floating point type
    *
    * Represents the F32 SQL data type.
    */
   class F32 : public Type {
   public:
      /** @brief Construct an F32 type
       * @param token The token representing F32
       */
      explicit F32(Token token) : Type{token} {}

      /** @brief Get the kind of type
       * @return TypeKind::F32
       */
      TypeKind kind() override { return TypeKind::F32; }
   };

   /** @brief 64-bit floating point type
    *
    * Represents the F64 SQL data type (double precision).
    */
   class F64 : public Type {
   public:
      /** @brief Construct an F64 type
       * @param token The token representing F64
       */
      explicit F64(Token token) : Type{token} {}

      /** @brief Get the kind of type
       * @return TypeKind::F64
       */
      TypeKind kind() override { return TypeKind::F64; }
   };

   /** @brief Variable-length string type
    *
    * Represents the VARCHAR SQL data type with a maximum size.
    */
   class Varchar : public Type {
   public:
      /** @brief Construct a Varchar type
       * @param size Maximum length of the string
       * @param token The token representing VARCHAR
       */
      explicit Varchar(size_t size, Token token) : _size{size}, Type{token} {}

      /** @brief Get the kind of type
       * @return TypeKind::Varchar
       */
      TypeKind kind() override { return TypeKind::Varchar; }

      /** @brief Get the maximum size of this varchar
       * @return The size in characters
       */
      size_t size() const { return _size; }

   private:
      size_t _size; ///< Maximum length of the string
   };

   /** @brief Fixed-length character type
    *
    * Represents the CHAR SQL data type with a fixed size.
    */
   class Char : public Type {
   public:
      /** @brief Construct a Char type
       * @param size Fixed length of the string
       * @param token The token representing CHAR
       */
      explicit Char(size_t size, Token token) : _size{size}, Type{token} {}

      /** @brief Get the kind of type
       * @return TypeKind::Char
       */
      TypeKind kind() override { return TypeKind::Char; }

      /** @brief Get the fixed size of this char
       * @return The size in characters
       */
      size_t size() const { return _size; }

   private:
      size_t _size; ///< Fixed length of the string
   };

   /** @brief Factory function for creating I32 types
    * @param token The token representing the I32 type
    * @return Shared pointer to a new I32 type
    */
   inline std::shared_ptr<I32> makeI32(Token token) { return std::make_shared<I32>(token); }

   /** @brief Factory function for creating U32 types
    * @param token The token representing the U32 type
    * @return Shared pointer to a new U32 type
    */
   inline std::shared_ptr<U32> makeU32(Token token) { return std::make_shared<U32>(token); }

   /** @brief Factory function for creating F32 types
    * @param token The token representing the F32 type
    * @return Shared pointer to a new F32 type
    */
   inline std::shared_ptr<F32> makeF32(Token token) { return std::make_shared<F32>(token); }

   /** @brief Factory function for creating F64 types
    * @param token The token representing the F64 type
    * @return Shared pointer to a new F64 type
    */
   inline std::shared_ptr<F64> makeF64(Token token) { return std::make_shared<F64>(token); }

   /** @brief Factory function for creating Varchar types
    * @param size Maximum length of the string
    * @param token The token representing the VARCHAR type
    * @return Shared pointer to a new Varchar type
    */
   inline std::shared_ptr<Varchar> makeVarchar(size_t size, Token token) { return std::make_shared<Varchar>(size, token); }

   /** @brief Factory function for creating Char types
    * @param size Fixed length of the string
    * @param token The token representing the CHAR type
    * @return Shared pointer to a new Char type
    */
   inline std::shared_ptr<Char> makeChar(size_t size, Token token) { return std::make_shared<Char>(size, token); }

} // namespace BadSQL