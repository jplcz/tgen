#ifndef EVALUATIONSCOPE_H
#define EVALUATIONSCOPE_H

#include "Context.h"
#include "Value.h"

namespace tgen {

class EvaluationScope {
public:
  explicit EvaluationScope(Context &ctx, EvaluationScope *parentScope = nullptr)
    : Ctx(ctx), Parent(parentScope) {
  }

  [[nodiscard]] EvaluationScope *getParent() const {
    return Parent;
  }

  [[nodiscard]] Context &getContext() const {
    return Ctx;
  }

  Value *getValue(const std::string_view &name);
  void setValue(const std::string_view &name, Value *value);

private:
  Context &Ctx;
  EvaluationScope *Parent;
  std::map<std::string, Value *, std::less<>> Values;
};

}

#endif //EVALUATIONSCOPE_H
