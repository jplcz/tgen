#include "ValueCast.h"

namespace tgen {

Value *ValueCast::castTo(Value *from, Type *toType) const {
  if (from->getType()->isOfType(toType))
    return from;

  if (from->getType()->getTypeID() == Type::TypeID::Void) {
    // Force evaluation
    from = from->evalulateInContext(Scope);
    if (from->getType()->isOfType(toType))
      return from;
  }

  switch (toType->getTypeID()) {
  case Type::TypeID::Int:
    if (auto result = castToInt(from); result)
      return result;
    break;
  case Type::TypeID::Bit:
    if (auto result = castToBit(from); result)
      return result;
    break;
  case Type::TypeID::String:
    if (auto result = castToString(from); result)
      return result;
    break;
  case Type::TypeID::List:
    break;
  case Type::TypeID::Class:
    break;
  case Type::TypeID::Bits:
    if (auto result = castToBits(from, toType); result)
      return result;
    break;
  case Type::TypeID::Void:
    return from;
  }

  throw std::runtime_error{
      fmt::format("Can't cast {} to {}", *from->getType(), *toType)};
}

Value *ValueCast::castToString(Value *from) const {
  switch (from->getType()->getTypeID()) {
  case Type::TypeID::Int:
    return StringValue::getStringValue(
        fmt::format("{}", must_cast<IntegerValue *>(from)->NumericValue),
        Scope.getContext());
  case Type::TypeID::Bit:
    return StringValue::getStringValue(
        fmt::format("{}", must_cast<BitValue *>(from)->Bit),
        Scope.getContext());
  case Type::TypeID::String:
    return from;
  case Type::TypeID::Bits:
    return StringValue::getStringValue(
        fmt::format("{}", must_cast<BitsValue *>(from)->Bits),
        Scope.getContext());
  case Type::TypeID::List:
  case Type::TypeID::Class:
  case Type::TypeID::Void:
    break;
  }

  return nullptr;
}

Value *ValueCast::castToInt(Value *from) const {
  switch (from->getType()->getTypeID()) {
  case Type::TypeID::Int:
    return from;
  case Type::TypeID::Bit:
    return IntegerValue::getIntegerValue(
        must_cast<BitValue *>(from)->Bit ? 1 : 0, Scope.getContext());
  case Type::TypeID::String:
    break;
  case Type::TypeID::List:
    break;
  case Type::TypeID::Class:
    break;
  case Type::TypeID::Bits:
    return IntegerValue::getIntegerValue(must_cast<BitsValue *>(from)->Bits,
                                         Scope.getContext());
  case Type::TypeID::Void:
    break;
  }

  return nullptr;
}

Value *ValueCast::castToBit(Value *from) const {
  switch(from->getType()->getTypeID()) {
  case Type::TypeID::Int:
    return BitValue::getBitValue(must_cast<IntegerValue *>(from)->NumericValue != 0, Scope.getContext());
  case Type::TypeID::Bit:
    return from;
  case Type::TypeID::String:
  case Type::TypeID::List:
  case Type::TypeID::Class:
    break;
  case Type::TypeID::Bits:
    return BitValue::getBitValue(must_cast<BitsValue *>(from)->Bits != 0,
                                         Scope.getContext());
  case Type::TypeID::Void:
    break;
  }

  return nullptr;
}

Value * ValueCast::castToBits(Value *from, Type *to) const {
  switch (from->getType()->getTypeID()) {
  case Type::TypeID::Int:
    return BitsValue::getBitsValue(must_cast<BitsType *>(to), must_cast<IntegerValue *>(from)->NumericValue);
  case Type::TypeID::Bit:
    return BitsValue::getBitsValue(must_cast<BitsType *>(to), must_cast<BitValue *>(from)->Bit ? 1 : 0);
  case Type::TypeID::String:
    break;
  case Type::TypeID::List:
    break;
  case Type::TypeID::Class:
    break;
  case Type::TypeID::Bits:
    return BitsValue::getBitsValue(must_cast<BitsType *>(to), must_cast<BitsValue *>(from)->Bits);
  case Type::TypeID::Void:
    break;
  }

  return nullptr;
}

}