#include "Type.h"
#include <stdexcept>
#include "ContextImpl.h"
#include "Value.h"
#include <ranges>

namespace tgen {

auto Type::CodeName(const Type::TypeID code) -> std::string_view {
  switch (code) {
#define TGEN_TYPE(Id, Name, BasicType) case TypeID::Name: return #Name;
#include "TGenTypes.inc"
#undef TGEN_TYPE
  default:
    throw std::runtime_error{"Unreachable code"};
  }
}

bool Type::CodeIsBasic(const TypeID code) {
  switch (code) {
#define TGEN_TYPE(Id, Name, BasicType) case TypeID::Name: return BasicType;
#include "TGenTypes.inc"
#undef TGEN_TYPE
  default:
    throw std::runtime_error{"Unreachable code"};
  }
}

Type::~Type() = default;

Type *Type::getIntTy(Context &context) {
  return &ContextImpl::get(context).IntTy;
}

Type *Type::getStringTy(Context &context) {
  return &ContextImpl::get(context).StringTy;
}

Type *Type::getBitTy(Context &context) {
  return &ContextImpl::get(context).BitTy;
}

Type *Type::getVoidTy(Context &context) {
  return &ContextImpl::get(context).VoidTy;
}

void Type::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "{}", CodeName(getTypeID()));
}

bool Type::isOfType(Type *type) const {
  switch (getTypeID()) {
  case TypeID::Int:
  case TypeID::Bit:
  case TypeID::String:
    return type->getTypeID() == getTypeID();
  default:
    throw std::runtime_error{"Derived"};
  }
}

ListType *ListType::getListType(Type *elType) {
  return ContextImpl::get(elType->getContext()).getListTy(elType);
}

ListType::~ListType() = default;

void ListType::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "list<{}>", *ElementType);
}

bool ListType::isOfType(Type *type) const {
  if (type->getTypeID() != TypeID::List)
    return false;
  return ElementType->
      isOfType(dynamic_cast<ListType *>(type)->getElementType());
}

void ClassMember::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "{}", *Ty);
  if (Name.has_value()) {
    fmt::format_to(ctx.out(), " {}", *Name);
  } else {
    fmt::format_to(ctx.out(), "<Unnamed>");
  }
  if (Initializer)
    fmt::format_to(ctx.out(), " = {}", *Initializer);
}

void ClassParameterDecl::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "{}", *Ty);
  if (Name.has_value())
    fmt::format_to(ctx.out(), "  {}", *Name);
  if (Initializer)
    fmt::format_to(ctx.out(), " = {}", *Initializer);
}

ClassMember * ClassType::findMember(const std::string_view &name) const {
  for (const auto &p : Members) {
    if (p->Name == name) {
      return p.get();
    }
  }
  if (ParentTy)
    return ParentTy->findMember(name);
  return nullptr;
}

ClassType *ClassType::getClassType(const std::string_view &name, Context &ctx) {
  return ContextImpl::get(ctx).getClassTy(name);
}

ClassType::~ClassType() = default;

void ClassType::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "Class:{}", Name);
  if (isParametrized()) {
    fmt::format_to(ctx.out(), "<{}>",
                   fmt::join(
                       Parameters | std::views::transform(
                           [](const auto &ptr) -> decltype(auto) {
                             return *ptr;
                           }), ", "));
  }
  if (IsDefined) {
    fmt::format_to(ctx.out(), " {{ {} }}", fmt::join(
                       Members | std::views::transform(
                           [](const auto &ptr) -> decltype(auto) {
                             return *ptr;
                           }), ", "));
  }
}

bool ClassType::isOfType(Type *type) const {
  if (type == this)
    return true;
  if (type->getTypeID() != TypeID::Class)
    return false;
  if (!ParentTy)
    return false;
  return ParentTy->isOfType(type);
}

void BitsType::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "bits<{}>", Width);
}

bool BitsType::isOfType(Type *type) const {
  return type->getTypeID() == TypeID::Bits &&
         dynamic_cast<BitsType *>(type)->Width == Width;
}

BitsType *BitsType::getBitsType(const unsigned int width, Context &ctx) {
  return ContextImpl::get(ctx).getBitsTy(width);
}

} // tgen
