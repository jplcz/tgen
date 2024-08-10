#ifndef TGEN_CONTEXTIMPL_H
#define TGEN_CONTEXTIMPL_H

#include "Context.h"
#include "Type.h"
#include "Value.h"
#include <map>
#include <memory>
#include <set>

namespace tgen {

class ContextImpl final : public Context {
public:
  Type IntTy, BitTy, StringTy, VoidTy;

  std::map<Type *, std::unique_ptr<ListType>> ListTypes;
  std::map<std::string, std::unique_ptr<ClassType>, std::less<>> ClassTypes;
  std::map<IntegerValue::IntegerTy, std::unique_ptr<IntegerValue>>
  IntegerValues;
  // Actual string is stored inside value node
  std::map<std::string_view, std::unique_ptr<StringValue>, std::less<>>
  StringValues;
  std::map<Type *, std::unique_ptr<ListValue>> EmptyListsOfType;

  std::set<std::unique_ptr<Value>> OtherValueTracker;

  BitValue TrueValue;
  BitValue FalseValue;
  StringValue EmptyString;

  std::map<unsigned int, std::unique_ptr<BitsType>> BitsTypes;

  ContextImpl();
  ~ContextImpl() override;

  static ContextImpl &get(Context &context) {
    return dynamic_cast<ContextImpl &>(context);
  }

  ListType *getListTy(Type *elType);
  ClassType *getClassTy(const std::string_view &name);
  IntegerValue *getIntegerV(IntegerValue::IntegerTy val);
  StringValue *getStringV(const std::string_view &value);
  ListValue *getListValue(Type *elementTy, std::span<Value *> values);
  BitsType *getBitsTy(unsigned int width);

};

} // tgen

#endif //TGEN_CONTEXTIMPL_H
