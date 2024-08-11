#include "ContextImpl.h"
#include "Type.h"
#include "TypeCasting.h"

namespace tgen {

ContextImpl::ContextImpl(): IntTy(Type::TypeID::Int, *this),
                            BitTy(Type::TypeID::Bit, *this),
                            StringTy(Type::TypeID::String, *this),
                            VoidTy(Type::TypeID::Void, *this),
                            TrueValue(true, *this),
                            FalseValue(false, *this),
                            EmptyString({}, *this) {
}

ContextImpl::~ContextImpl() = default;

ListType *ContextImpl::getListTy(Type *elType) {
  const auto it = ListTypes.find(elType);
  if (it == ListTypes.end()) {
    auto [insInter, inserted] = ListTypes.emplace(
        elType, std::make_unique<ListType>(elType, *this));
    return insInter->second.get();
  }
  return it->second.get();
}

ClassType *ContextImpl::getClassTy(const std::string_view &name) {
  const auto it = ClassTypes.find(name);
  if (it == ClassTypes.end()) {
    auto [insInter, inserted] = ClassTypes.emplace(
        name, std::make_unique<ClassType>(std::string(name), *this));
    return insInter->second.get();
  }
  return it->second.get();
}

IntegerValue *ContextImpl::getIntegerV(const IntegerValue::IntegerTy val) {
  const auto it = IntegerValues.find(val);
  if (it == IntegerValues.end()) {
    auto [insInter, inserted] = IntegerValues.emplace(
        val, std::make_unique<IntegerValue>(val, *this));
    return insInter->second.get();
  }
  return it->second.get();
}

StringValue *ContextImpl::getStringV(const std::string_view &value) {
  if (value.empty())
    return &EmptyString;
  auto it = StringValues.find(value);
  if (it == StringValues.end()) {
    auto instance = std::make_unique<StringValue>(std::string(value), *this);
    auto [insIter, inserted] = StringValues.emplace(
        instance->String, std::move(instance));
    return insIter->second.get();
  }
  return it->second.get();
}

ListValue *ContextImpl::getListValue(Type *elementTy,
                                     std::span<Value *> values) {
  if (values.empty()) {
    auto it = EmptyListsOfType.find(elementTy);
    if (it != EmptyListsOfType.end())
      return it->second.get();
    auto [insIter, inserted] = EmptyListsOfType.emplace(
        elementTy,
        std::make_unique<ListValue>(elementTy, std::vector<Value *>(), *this));
    return insIter->second.get();
  }

  auto [insIter, inserted] = OtherValueTracker.insert(
      std::make_unique<ListValue>(
          elementTy, std::vector<Value *>(values.begin(), values.end()),
          *this));

  return must_cast<ListValue *>(insIter->get());
}

BitsType *ContextImpl::getBitsTy(unsigned int width) {
  auto it = BitsTypes.find(width);
  if (it == BitsTypes.end()) {
    auto [insInter, inserted] = BitsTypes.emplace(
        width, std::make_unique<BitsType>(width, *this));
    return insInter->second.get();
  }
  return it->second.get();
}

BitsValue *ContextImpl::getBitsValue(BitsType *ty, BitsValue::BitsT value) {
  const auto key = std::make_tuple(ty, value);
  if (const auto it = BitsValues.find(key); it != BitsValues.end())
    return it->second.get();
  auto [insIter, inserted] = BitsValues.emplace(key,
      std::make_unique<BitsValue>(ty, value, *this));
  return insIter->second.get();
}

} // tgen