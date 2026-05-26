#pragma once

#include <memory>
#include "token.h"

namespace BadSQL {
   enum class TypeKind {
      I32,
      U32,
      F32,
      F64,
      Varchar,
      Char
   };

   class Type {
   public:
      explicit Type(Token token) : _Token{token} {}
      virtual ~Type() = default;
      virtual TypeKind kind() = 0;

   protected:
      Token _Token;
   };

   class I32 : public Type {
   public:
      explicit I32(Token token) : Type{token} {}
      TypeKind kind() override { return TypeKind::I32; }
   };

   class U32 : public Type {
   public:
      explicit U32(Token token) : Type{token} {}
      TypeKind kind() override { return TypeKind::U32; }
   };

   class F32 : public Type {
   public:
      explicit F32(Token token) : Type{token} {}
      TypeKind kind() override { return TypeKind::F32; }
   };

   class F64 : public Type {
   public:
      explicit F64(Token token) : Type{token} {}
      TypeKind kind() override { return TypeKind::F64; }
   };

   class Varchar : public Type {
   public:
      explicit Varchar(size_t size, Token token ) : _size{size}, Type{token} {}
      TypeKind kind() override { return TypeKind::Varchar; }
      size_t size() const { return _size; }

   private:
      size_t _size;
   };

   class Char : public Type {
   public:
      explicit Char(size_t size, Token token) : _size{size}, Type{token} {}
      TypeKind kind() override { return TypeKind::Char; }
      size_t size() const { return _size; }

   private:
      size_t _size;
   };

   inline std::shared_ptr<I32> makeI32(Token token) { return std::make_shared<I32>(token); }
   inline std::shared_ptr<U32> makeU32(Token token) { return std::make_shared<U32>(token); }
   inline std::shared_ptr<F32> makeF32(Token token) { return std::make_shared<F32>(token); }
   inline std::shared_ptr<F64> makeF64(Token token) { return std::make_shared<F64>(token); }
   inline std::shared_ptr<Varchar> makeVarchar(size_t size, Token token) { return std::make_shared<Varchar>(size, token); }
   inline std::shared_ptr<Char> makeChar(size_t size, Token token) { return std::make_shared<Char>(size, token); }

} // namespace BadSQL