#ifndef EXPR_H
#define EXPR_H

#include "Value.h"
#include "EvaluationScope.h"

namespace tgen {

class Expr : public Value {
public:
  enum class ExprType {
#define TGEN_EXPR(Id, Name, ArgCount) Name = Id,
#include "Expr.inc"
#undef TGEN_EXPR
  };

  static std::string_view ExprTypeName(ExprType type);
  static unsigned ExprArgumentCount(ExprType type);

  const ExprType TypeCode;
  std::vector<Value *> Arguments;

  Expr(Type *resultTy, ExprType exprType, Context &ctx);
  ~Expr() override;

  void format_to(fmt::format_context &ctx) const override;
  [[nodiscard]] bool isExpression() const final { return true; };
};

class ValueExpr final : public Expr {
  Value *SourceValue;

public:
  explicit ValueExpr(Value *sourceValue) :
    Expr(sourceValue->getType(), ExprType::Value, sourceValue->getContext()),
    SourceValue(sourceValue) {
  }

  Value *evalulateInContext(EvaluationScope &scope) override;
};

class BinOpExpr final : public Expr {
public:
  BinOpExpr(Value *left, Value *right, ExprType type) :
    Expr(left->getType(), type, left->getContext()) {
    Arguments.push_back(left);
    Arguments.push_back(right);
  }

  [[nodiscard]] Value *evalulateInContext(EvaluationScope &scope) override;
};

} // tgen

#endif //EXPR_H
