#include "Context.h"
#include "Type.h"
#include "EvaluationScope.h"

#include <iostream>

int main() {
  auto ctx = tgen::Context::createContext();

  auto scope = std::make_unique<tgen::EvaluationScope>(*ctx);

  auto instFormat = tgen::ClassType::getClassType("InstFormat", *ctx);

  instFormat->setParametrized(true);
  instFormat->addParameterDecl(tgen::ClassParameterDecl(
      tgen::BitsType::getBitsType(5, *ctx),
      "val",
      nullptr
      ));

  instFormat->setDefined(true);

  scope->setValue("SchedMxList",
                  tgen::ListValue::getListValueFromInit(
                      tgen::Type::getStringTy(*ctx),
                      tgen::StringValue::getStringValue("M1", *ctx),
                      tgen::StringValue::getStringValue("M2", *ctx),
                      tgen::StringValue::getStringValue("M4", *ctx),
                      tgen::StringValue::getStringValue("M8", *ctx)
                      ));



  SPDLOG_INFO("{}", *scope->getValue("SchedMxList"));
  SPDLOG_INFO("{}", *instFormat);

  return 0;
}
