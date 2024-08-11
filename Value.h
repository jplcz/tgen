#ifndef VALUE_H
#define VALUE_H

#include "Type.h"
#include <numeric>
#include <span>

namespace tgen {

class EvaluationScope;

class Value {
  Type *ValueType = nullptr;
  Context &Ctx;

public:
  Value(Type *valueTy, Context &ctx) :
    ValueType(valueTy), Ctx(ctx) {
  }

  virtual ~Value();

  [[nodiscard]] Type *getType() const { return ValueType; }

  [[nodiscard]] Context &getContext() const { return Ctx; }

  virtual auto format_to(fmt::format_context &ctx) const -> void;
  [[nodiscard]] virtual bool isExpression() const { return false; }
  [[nodiscard]] virtual Value *evalulateInContext(EvaluationScope &scope);
};

class IntegerValue final : public Value {
public:
  using IntegerTy = std::intmax_t;

  const IntegerTy NumericValue;

  IntegerValue(const IntegerTy numericValue, Context &ctx) :
    Value(Type::getIntTy(ctx), ctx), NumericValue(numericValue) {
  }

  void format_to(fmt::format_context &ctx) const override;

  static IntegerValue *getIntegerValue(IntegerTy numericValue, Context &ctx);
  Value *evalulateInContext(EvaluationScope &scope) override;
};

class BitValue final : public Value {
public:
  const bool Bit;

  BitValue(const bool bit, Context &ctx) : Value(Type::getBitTy(ctx), ctx),
                                           Bit(bit) {
  }

  void format_to(fmt::format_context &ctx) const override;

  static BitValue *getBitValue(bool bitValue, Context &ctx);
  Value *evalulateInContext(EvaluationScope &scope) override;
};

class StringValue final : public Value {
public:
  const std::string String;

  StringValue(std::string str, Context &ctx) :
    Value(Type::getStringTy(ctx), ctx), String(std::move(str)) {
  }

  void format_to(fmt::format_context &ctx) const override;

  static StringValue *getStringValue(const std::string_view &str, Context &ctx);
  Value *evalulateInContext(EvaluationScope &scope) override;
};

class ListValue final : public Value {
  const std::vector<Value *> Values;

public:
  ListValue(Type *elementTy, std::vector<Value *> values, Context &ctx) :
    Value(ListType::getListType(elementTy), ctx), Values(std::move(values)) {
    for (const auto &item : Values) {
      if (!item->getType()->isOfType(elementTy))
        throw std::runtime_error{"Invalid types"};
    }
  }

  [[nodiscard]] std::size_t getElementCount() const {
    return Values.size();
  }

  [[nodiscard]] Value *getElement(const std::size_t index) const {
    return Values.at(index);
  }

  void format_to(fmt::format_context &ctx) const override;

  static ListValue *getListValue(Type *elementTy, std::span<Value *> values);

  template <typename... Args>
  static ListValue *getListValueFromInit(Type *elementTy, Args &&... args) {
    std::vector<Value *> vec{{args...}};
    return getListValue(elementTy, vec);
  }

  Value *evalulateInContext(EvaluationScope &scope) override;
};

class BitsValue : public Value {
public:
  using BitsT = uintmax_t;

  const BitsT Bits;

  BitsValue(BitsType *ty, const BitsT bits, Context &ctx) : Value(
      ty, ctx), Bits(bits) {
  }

  void format_to(fmt::format_context &ctx) const override;

  static BitsValue *getBitsValue(BitsType *ty, BitsT bits);
};

class ClassDef : public Value {
  std::string Name;
  std::vector<Value *> ParameterValues;

public:
  ClassDef(ClassType *classTy, Context &ctx, std::string name,
           std::vector<Value *> parameterValues) :
    Value(classTy, ctx), Name(std::move(name)),
    ParameterValues(std::move(parameterValues)) {
  }

  void format_to(fmt::format_context &ctx) const override;

  [[nodiscard]] static ClassDef *
  getClassDef(ClassType *classTy, const std::string_view &name,
              std::span<Value *> parameterInitializer);
};

} // tgen

template <typename Ty>
struct fmt::formatter<
      Ty, char, std::enable_if_t<
        std::is_base_of_v<
          tgen::Value, Ty>>> : fmt::formatter<std::string_view> {
  template <typename FormatContext>
  static auto format(const tgen::Value &value,
                     FormatContext &ctx) -> decltype(ctx.out()) {
    value.format_to(ctx);
    return ctx.out();
  }
};

#endif //VALUE_H