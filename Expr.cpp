#include "Expr.h"

#include "ContextImpl.h"

#include <ranges>

namespace tgen {

std::string_view Expr::ExprTypeName(ExprType type) {
  switch (type) {
#define TGEN_EXPR(Id, Name, ArgCount) case ExprType::Name: return #Name;
#include "Expr.inc"
#undef TGEN_EXPR
  default:
    throw std::runtime_error{"Unreachable"};
  }
}

unsigned Expr::ExprArgumentCount(ExprType type) {
  switch (type) {
#define TGEN_EXPR(Id, Name, ArgCount) case ExprType::Name: return ArgCount;
#include "Expr.inc"
#undef TGEN_EXPR
  default:
    throw std::runtime_error{"Unreachable"};
  }
}

Expr::Expr(Type *resultTy, ExprType exprType, Context &ctx) :
  Value(resultTy, ctx), TypeCode(exprType) {
}

Expr::~Expr() = default;

void Expr::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "{}:{}", ExprTypeName(TypeCode), *getType());
  if (Arguments.size() > 1) {
    fmt::format_to(ctx.out(), " {}", fmt::join(
                       Arguments | std::views::transform(
                           [](const auto &ptr) -> decltype(auto) {
                             return *ptr;
                           }), ", "));
  }
}

Expr *Expr::TrackExpression(Context &context, std::unique_ptr<Expr> expr) {
  return must_cast<Expr *>(ContextImpl::get(context).OtherValueTracker.
    emplace(std::move(expr)).
    first->get());
}

Value *ValueExpr::evalulateInContext(EvaluationScope &scope) {
  return SourceValue->evalulateInContext(scope);
}

Value *IdentifierExpr::evalulateInContext(EvaluationScope &scope) {
  return scope.evaluateIdentifier(Name);
}

void IdentifierExpr::format_to(fmt::format_context &ctx) const {
  fmt::format_to(ctx.out(), "GetValue({})", Name);
}

} // tgen