#include "Context.h"
#include "Type.h"
#include "EvaluationScope.h"
#include "Expr.h"

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
  instFormat->addMember(tgen::ClassMember(tgen::BitsType::getBitsType(5, *ctx),
                                          "Value",
                                          tgen::Expr::MakeExpr<
                                            tgen::IdentifierExpr>(
                                              *ctx, "val", *ctx)
      ));

  scope->setValue("SchedMxList",
                  tgen::ListValue::getListValueFromInit(
                      tgen::Type::getStringTy(*ctx),
                      tgen::StringValue::getStringValue("M1", *ctx),
                      tgen::StringValue::getStringValue("M2", *ctx),
                      tgen::StringValue::getStringValue("M4", *ctx),
                      tgen::StringValue::getStringValue("M8", *ctx)
                      ));

  std::array<tgen::Value *, 1> params{};
  params[0] = tgen::BitsValue::getBitValue();

  auto InstFormatPseudo = tgen::ClassDef::getClassDef(
      instFormat, "InstFormatPseudo",
      params);

  scope->setValue("InstFormatPseudo",
                  InstFormatPseudo);

  SPDLOG_INFO("{}", *scope->getValue("SchedMxList"));
  SPDLOG_INFO("{}", *instFormat);
  SPDLOG_INFO("{}", *InstFormatPseudo);

  return 0;
}
