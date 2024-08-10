#include "EvaluationScope.h"

#include <ranges>

namespace tgen {

Value *EvaluationScope::getValue(const std::string_view &name) {
  auto it = Values.find(name);
  if (it != Values.end())
    return it->second;
  return it->second;
}

void EvaluationScope::setValue(const std::string_view &name, Value *value) {
  Values[std::string(name)] = value;
}

Value *EvaluationScope::evaluateIdentifier(const std::string_view &name) {
  return getValue(name);
}

}