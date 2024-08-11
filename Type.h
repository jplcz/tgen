#ifndef TGEN_TYPE_H
#define TGEN_TYPE_H

#include "Context.h"
#include "TypeCasting.h"

#include <spdlog/spdlog.h>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace tgen {

class Context;
class Value;

class Type {
public:
  enum class TypeID {
#define TGEN_TYPE(Id, Name, BasicType) Name = Id,
#include "TGenTypes.inc"
#undef TGEN_TYPE
  };

private:
  const TypeID TypeCode;
  Context &Ctx;

public:
  [[nodiscard]] std::string_view getCodeName() const {
    return CodeName(TypeCode);
  }

  [[nodiscard]] TypeID getTypeID() const {
    return TypeCode;
  }

  [[nodiscard]] Context &getContext() const {
    return Ctx;
  }

  [[nodiscard]] bool isBasic() const {
    return CodeIsBasic(TypeCode);
  }

  [[nodiscard]] static std::string_view CodeName(TypeID code);
  [[nodiscard]] static bool CodeIsBasic(TypeID code);

  explicit Type(const TypeID code, Context &ctx) : TypeCode(code), Ctx(ctx) {
  }

  virtual ~Type();

  static Type *getIntTy(Context &context);
  static Type *getStringTy(Context &context);
  static Type *getBitTy(Context &context);
  static Type *getVoidTy(Context &context);

  virtual void format_to(fmt::format_context &ctx) const;

  virtual bool isOfType(Type *type) const;
};

class ListType : public Type {
  Type *ElementType;

public:
  ListType(Type *elType, Context &ctx) : Type(TypeID::List, ctx),
                                         ElementType(elType) {
  }

  [[nodiscard]] Type *getElementType() const { return ElementType; }

  static ListType *getListType(Type *elType);
  ~ListType() override;

  void format_to(fmt::format_context &ctx) const override;
  bool isOfType(Type *type) const override;
};

struct ClassMember {
  Type *Ty = nullptr;
  std::string Name;
  Value *Initializer = nullptr;

  ClassMember(Type *ty, std::string name, Value *initializer) :
    Ty(ty), Name(std::move(name)), Initializer(initializer) {
  }

  void format_to(fmt::format_context &ctx) const;
};

struct ClassParameterDecl {
  Type *Ty = nullptr;
  std::optional<std::string> Name;
  Value *Initializer = nullptr;

  ClassParameterDecl(Type *ty, std::optional<std::string> name,
                     Value *initializer) :
    Ty(ty), Name(std::move(name)), Initializer(initializer) {
  }

  void format_to(fmt::format_context &ctx) const;
};

class ClassType : public Type {
private:
  std::string Name;
  std::vector<std::unique_ptr<ClassMember>> Members;
  std::vector<std::unique_ptr<ClassParameterDecl>> Parameters;
  bool IsDefined = false;
  bool IsParametrized = false;
  ClassType *ParentTy = nullptr;

public:
  ClassType(std::string name, Context &ctx):
    Type(TypeID::Class, ctx), Name(std::move(name)) {
  }

  ~ClassType() override;

  [[nodiscard]] std::string getName() const {
    return Name;
  }

  [[nodiscard]] bool isDefined() const {
    return IsDefined;
  }

  void setDefined(const bool newDefined) noexcept {
    IsDefined = newDefined;
  }

  [[nodiscard]] bool isParametrized() const { return IsParametrized; }

  void setParametrized(const bool newParametrized) noexcept {
    IsParametrized = newParametrized;
  }

  [[nodiscard]] Type *getParentType() const {
    return ParentTy;
  }

  void setParentType(ClassType *newParent) {
    ParentTy = newParent;
  }

  [[nodiscard]] ClassParameterDecl *findParameterDecl(
      const std::string_view &decl) const {
    std::size_t nest = 0;
    for (auto self = this; self; self = self->ParentTy) {
      ++nest;
      if (nest > 100)
        throw std::runtime_error{"Too many types in hierarchy"};
      for (const auto &p : self->Parameters) {
        if (p->Name == decl) {
          return p.get();
        }
      }
    }
    return nullptr;
  }

  void addParameterDecl(const ClassParameterDecl &decl) {
    Parameters.emplace_back(std::make_unique<ClassParameterDecl>(decl));
  }

  [[nodiscard]] ClassMember *findMember(
      const std::string_view &name) const;

  void addMember(const ClassMember &member) {
    Members.emplace_back(std::make_unique<ClassMember>(member));
  }

  static ClassType *getClassType(const std::string_view &name, Context &ctx);

  void format_to(fmt::format_context &ctx) const override;
  bool isOfType(Type *type) const override;
};

class BitsType : public Type {
public:
  unsigned int Width;

  BitsType(const unsigned int width, Context &ctx) : Type(TypeID::Bits, ctx),
    Width(width) {
  }

  void format_to(fmt::format_context &ctx) const override;
  bool isOfType(Type *type) const override;

  static BitsType *getBitsType(unsigned int width, Context &ctx);
};

} // tgen

template <typename Ty>
struct fmt::formatter<
      Ty, char, std::enable_if_t<
        std::is_base_of_v<
          tgen::Type, Ty>>> : fmt::formatter<std::string_view> {
  template <typename FormatContext>
  static auto format(const tgen::Type &value,
                     FormatContext &ctx) -> decltype(ctx.out()) {
    value.format_to(ctx);
    return ctx.out();
  }
};

template <> struct fmt::formatter<
      tgen::ClassMember> : fmt::formatter<std::string_view> {
  template <typename FormatContext>
  static auto format(const tgen::ClassMember &value,
                     FormatContext &ctx) -> decltype(ctx.out()) {
    value.format_to(ctx);
    return ctx.out();
  }
};

template <> struct fmt::formatter<
      tgen::ClassParameterDecl> : fmt::formatter<std::string_view> {
  template <typename FormatContext>
  static auto format(const tgen::ClassParameterDecl &value,
                     FormatContext &ctx) -> decltype(ctx.out()) {
    value.format_to(ctx);
    return ctx.out();
  }
};

#endif //TGEN_TYPE_H
