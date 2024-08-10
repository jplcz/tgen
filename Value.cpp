#include "Value.h"

#include "ContextImpl.h"

namespace tgen {

Value::~Value() = default;

void Value::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "Val:{}", *ValueType);
}

void IntegerValue::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "{}:{}", *getType(), NumericValue);
}

IntegerValue *IntegerValue::getIntegerValue(const IntegerTy numericValue,
                                            Context &ctx) {
  return ContextImpl::get(ctx).getIntegerV(numericValue);
}

void BitValue::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "{}:{}", *getType(), Bit ? "true" : "false");
}

BitValue *BitValue::getBitValue(const bool bitValue, Context &ctx) {
  auto &ctxRef = ContextImpl::get(ctx);
  return bitValue ? &ctxRef.TrueValue : &ctxRef.FalseValue;
}

void StringValue::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "{}:\"{}\"", *getType(), String);
}

StringValue *StringValue::getStringValue(const std::string_view &str,
                                         Context &ctx) {
  return ContextImpl::get(ctx).getStringV(str);
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

void ClassDef::format_to(fmt::format_context &ctx) const {
  Value::format_to(ctx);
}

} // tgen