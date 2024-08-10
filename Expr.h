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

  template <typename Derived, typename... Args>
  [[nodiscard]] static Derived *MakeExpr(Context &context, Args &&... args) {
    auto expr = std::make_unique<Derived>(std::forward<Args>(args)...);
    return static_cast<Derived *>(TrackExpression(context, std::move(expr)));
  }

private:
  [[nodiscard]] static Expr *TrackExpression(Context &context,
                                             std::unique_ptr<Expr> expr);
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
  BinOpExpr(Value *left, Value *right, const ExprType type) :
    Expr(left->getType(), type, left->getContext()) {
    Arguments.push_back(left);
    Arguments.push_back(right);
  }
};

class IdentifierExpr final : public Expr {
  const std::string Name;

public:
  IdentifierExpr(std::string name, Context &context) :
    Expr(Type::getVoidTy(context), ExprType::Identifier, context),
    Name(std::move(name)) {
  }

  [[nodiscard]] std::string_view getName() const { return Name; }

  [[nodiscard]] Value * evalulateInContext(EvaluationScope &scope) override;
  void format_to(fmt::format_context &ctx) const override;
};

} // tgen

#endif //EXPR_H
