#include "Value.h"

#include "ContextImpl.h"

namespace tgen {

Value::~Value() = default;

void Value::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "Val:{}", *ValueType);
}

Value *Value::evalulateInContext(EvaluationScope &) {
  throw std::runtime_error(
      "Value can't be evaludated");
}

void IntegerValue::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "{}:{}", *getType(), NumericValue);
}

IntegerValue *IntegerValue::getIntegerValue(const IntegerTy numericValue,
                                            Context &ctx) {
  return ContextImpl::get(ctx).getIntegerV(numericValue);
}

Value *IntegerValue::evalulateInContext(EvaluationScope &) {
  return this;
}

void BitValue::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "{}:{}", *getType(), Bit ? "true" : "false");
}

BitValue *BitValue::getBitValue(const bool bitValue, Context &ctx) {
  auto &ctxRef = ContextImpl::get(ctx);
  return bitValue ? &ctxRef.TrueValue : &ctxRef.FalseValue;
}

Value *BitValue::evalulateInContext(EvaluationScope &) {
  return this;
}

void StringValue::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "{}:\"{}\"", *getType(), String);
}

StringValue *StringValue::getStringValue(const std::string_view &str,
                                         Context &ctx) {
  return ContextImpl::get(ctx).getStringV(str);
}

Value *StringValue::evalulateInContext(EvaluationScope &) {
  return this;
}

void ListValue::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "{} [", *getType());

  if (!Values.empty()) {
    fmt::format_to(ctx.out(), " ");
    bool first = true;
    for (const auto &item : Values) {
      if (!first) {
        fmt::format_to(ctx.out(), ", ");
      } else {
        first = false;
      }
      fmt::format_to(ctx.out(), "{}", *item);
    }
    fmt::format_to(ctx.out(), " ");
  }

  fmt::format_to(ctx.out(), "]");
}

ListValue *ListValue::getListValue(Type *elementTy,
                                   const std::span<Value *> values) {
  return ContextImpl::get(elementTy->getContext()).getListValue(
      elementTy, values);
}

Value *ListValue::evalulateInContext(EvaluationScope &) {
  return this;
}

void BitsValue::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "{}:{:#x}", *getType(), Bits);
}

BitsValue * BitsValue::getBitsValue(BitsType *ty, BitsT bits) {
  return ContextImpl::get(ty->getContext()).getBitsValue(ty, bits);
}

void ClassDef::format_to(fmt::format_context &ctx) const {
  Value::format_to(ctx);
}

ClassDef *ClassDef::getClassDef(ClassType *classTy,
                                const std::string_view &name,
                                std::span<Value *> parameterInitializer) {
  auto def = std::make_unique<ClassDef>(classTy,
                                        classTy->getContext(),
                                        std::string(name),
                                        std::vector<Value *>(
                                            parameterInitializer.begin(),
                                            parameterInitializer.end()));
  auto [insIter, insResult] = ContextImpl::get(classTy->getContext()).
                              OtherValueTracker.insert(std::move(def));
  return must_cast<ClassDef *>(insIter->get());
}

} // tgen